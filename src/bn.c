#include "core.h"

// 返回最高位比特为1的位置
int util_bits_dig(dig_t a){
    int ans = 0;
    while (a)
    {
        a >>= 1;
        ans++;
    }
    return ans;
}

dig_t bn_addn_low(dig_t *c, const dig_t *a, const dig_t *b, int size) {
	int i;
	register dig_t carry, c0, c1, r0, r1;

	carry = 0;
	for (i = 0; i < size; i++, a++, b++, c++) {
		r0 = (*a) + (*b);
		c0 = (r0 < (*a));
		r1 = r0 + carry;
		c1 = (r1 < r0);
		carry = c0 | c1;
		(*c) = r1;
	}
	return carry;
}

dig_t bn_add1_low(dig_t *c, const dig_t *a, dig_t digit, int size) {
	int i;
	register dig_t carry, r0;

	carry = digit;
	for (i = 0; i < size && carry; i++, a++, c++) {
		r0 = (*a) + carry;
		carry = (r0 < carry);
		(*c) = r0;
	}
	for (; i < size; i++, a++, c++) {
		(*c) = (*a);
	}
	return carry;
}

dig_t bn_subn_low(dig_t *c, const dig_t *a, const dig_t *b, int size) {
	int i;
	dig_t carry, r0, diff;

	/* Zero the carry. */
	carry = 0;
	for (i = 0; i < size; i++, a++, b++, c++) {
		diff = (*a) - (*b);
		r0 = diff - carry;
		carry = ((*a) < (*b)) || (carry && !diff);
		(*c) = r0;
	}
	return carry;
}

dig_t bn_sub1_low(dig_t *c, const dig_t *a, dig_t digit, int size) {
	int i;
	dig_t carry, r0;

	carry = digit;
	for (i = 0; i < size && carry; i++, c++, a++) {
		r0 = (*a) - carry;
		carry = (r0 > (*a));
		(*c) = r0;
	}
	for (; i < size; i++, a++, c++) {
		(*c) = (*a);
	}
	return carry;
}

dig_t bn_lshb_low(dig_t *c, const dig_t *a, int size, int bits) {
	int i;
	dig_t r, carry, shift, mask;

	shift = RLC_DIG - bits;
	carry = 0;
	mask = RLC_MASK(bits);
	for (i = 0; i < size; i++, a++, c++) {
		/* Get the needed least significant bits. */
		r = ((*a) >> shift) & mask;
		/* Shift left the operand. */
		*c = ((*a) << bits) | carry;
		/* Update the carry. */
		carry = r;
	}
	return carry;
}

dig_t bn_rshb_low(dig_t *c, const dig_t *a, int size, int bits) {
	int i;
	dig_t r, carry, shift, mask;

	c += size - 1;
	a += size - 1;
	/* Prepare the bit mask. */
	shift = (RLC_DIG - bits) % RLC_DIG;
	carry = 0;
	mask = RLC_MASK(bits);
	for (i = size - 1; i >= 0; i--, a--, c--) {
		/* Get the needed least significant bits. */
		r = (*a) & mask;
		/* Shift left the operand. */
		*c = ((*a) >> bits) | (carry << shift);
		/* Update the carry. */
		carry = r;
	}
	return carry;
}

dig_t bn_mul1_low(dig_t *c, const dig_t *a, dig_t digit, int size) {
	dig_t r0, r1, carry = 0;
	for (int i = 0; i < size; i++, a++, c++) {
		RLC_MUL_DIG(r1, r0, *a, digit);
		*c = r0 + carry;
		carry = r1 + (*c < carry);
	}
	return carry;
}

void bn_divn_low(dig_t *c, dig_t *d, dig_t *a, int sa, dig_t *b, int sb) {
	int norm, i, n, t, sd;
	dig_t carry, t1[3], t2[3];

	/* Normalize x and y so that the leading digit of y is bigger than
	 * 2^(RLC_DIG-1). */
	norm = util_bits_dig(b[sb - 1]) % RLC_DIG;

	if (norm < (int)(RLC_DIG - 1)) {
		norm = (RLC_DIG - 1) - norm;
		carry = bn_lshb_low(a, a, sa, norm);
		if (carry) {
			a[sa++] = carry;
		}
		carry = bn_lshb_low(b, b, sb, norm);
		if (carry) {
			b[sb++] = carry;
		}
	} else {
		norm = 0;
	}

	n = sa - 1;
	t = sb - 1;

	/* Shift y so that the most significant digit of y is aligned with the
	 * most significant digit of x. */
	dv_lshd(b, b, sb + (n - t), (n - t));

	/* Find the most significant digit of the quotient. */
	while (dv_cmp(a, b, sa) != RLC_LT) {
		c[n - t]++;
		bn_subn_low(a, a, b, sa);
	}

	/* Shift y back. */
	dv_rshd(b, b, sb + (n - t), (n - t));

	/* Find the remaining digits. */
	for (i = n; i >= (t + 1); i--) {
		if (i > sa) {
			continue;
		}

		if (a[i] == b[t]) {
			c[i - t - 1] = RLC_MASK(RLC_DIG);
		} else {
			RLC_DIV_DIG(c[i - t - 1], carry, a[i], a[i - 1], b[t]);
		}

		c[i - t - 1]++;
		do {
			c[i - t - 1]--;
			t1[0] = (t - 1 < 0) ? 0 : b[t - 1];
			t1[1] = b[t];

			carry = bn_mul1_low(t1, t1, c[i - t - 1], 2);
			t1[2] = carry;

			t2[0] = (i - 2 < 0) ? 0 : a[i - 2];
			t2[1] = (i - 1 < 0) ? 0 : a[i - 1];
			t2[2] = a[i];
		} while (dv_cmp(t1, t2, 3) == RLC_GT);

		carry = bn_mul1_low(d, b, c[i - t - 1], sb);
		sd = sb;
		if (carry) {
			d[sd++] = carry;
		}

		carry = bn_subn_low(a + (i - t - 1), a + (i - t - 1), d, sd);
		sd += (i - t - 1);
		if (sa - sd > 0) {
			carry = bn_sub1_low(a + sd, a + sd, carry, sa - sd);
		}

		if (carry) {
			sd = sb + (i - t - 1);
			carry = bn_addn_low(a + (i - t - 1), a + (i - t - 1), b, sb);
			carry = bn_add1_low(a + sd, a + sd, carry, sa - sd);
			c[i - t - 1]--;
		}
	}
	/* Remainder should be not be longer than the divisor. */
	bn_rshb_low(d, a, sb, norm);
}