#include "core.h"
#include "fp.h"

dig_t fp_addn_low(dig_t *c, const dig_t *a, const dig_t *b) {
	int i;
	dig_t carry, c0, c1, r0, r1;

	carry = 0;
	for (i = 0; i < RLC_FP_DIGS; i++, a++, b++, c++) {
		r0 = (*a) + (*b);
		c0 = (r0 < (*a));
		r1 = r0 + carry;
		c1 = (r1 < r0);
		carry = c0 | c1;
		(*c) = r1;
	}
	return carry;
}

dig_t fp_subn_low(dig_t *c, const dig_t *a, const dig_t *b) {
	// printf("111\n");
	int i;
	dig_t carry, r0, diff;

	/* Zero the carry. */
	carry = 0;
	for (i = 0; i < RLC_FP_DIGS; i++, a++, b++, c++) {
		diff = (*a) - (*b);
		r0 = diff - carry;
		carry = ((*a) < (*b)) || (carry && !diff);
		(*c) = r0;
	}
	return carry;
}

dig_t fp_mula_low(dig_t *c, const dig_t *a, dig_t digit) {
	dig_t _c, r0, r1, carry = 0;
	for (int i = 0; i < RLC_FP_DIGS; i++, a++, c++) {
		/* Multiply the digit *a by d and accumulate with the previous
		 * result in the same columns and the propagated carry. */
		RLC_MUL_DIG(r1, r0, *a, digit);
		_c = r0 + carry;
		carry = r1 + (_c < carry);
		/* Increment the column and assign the result. */
		*c = *c + _c;
		/* Update the carry. */
		carry += (*c < _c);
	}
	return carry;
}

dig_t fp_lsh1_low(dig_t *c, const dig_t *a) {
	int i;
	dig_t r, carry;

	/* Prepare the bit mask. */
	carry = 0;
	for (i = 0; i < RLC_FP_DIGS; i++, a++, c++) {
		/* Get the most significant bit. */
		r = *a >> (RLC_DIG - 1);
		/* Shift the operand and insert the carry, */
		*c = (*a << 1) | carry;
		/* Update the carry. */
		carry = r;
	}
	return carry;
}