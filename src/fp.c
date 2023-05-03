#include "fp.h"
#include "dv.h"
#include "core.h"

dig_t* fp_prime_get(){
	return FP_PRIME;
}

void fp_zero(fp_t a) {
	dv_zero(a, RLC_FP_DIGS);
}

int fp_is_zero(const fp_t a) {
	int i;
	dig_t t = 0;

	for (i = 0; i < RLC_FP_DIGS; i++) {
		t |= a[i];
	}

	return !t;
}

void  fp_add_basic(fp_t c, const fp_t a, const fp_t b) {
	dig_t carry;
	carry = fp_addn_low(c, a, b);
	if (carry || (dv_cmp(c, fp_prime_get(), RLC_FP_DIGS) != RLC_LT)) {
		carry = fp_subn_low(c, c, fp_prime_get());
	}
}

void fp_sub_basic(fp_t c, const fp_t a, const fp_t b) {
	dig_t carry;
	carry = fp_subn_low(c, a, b);
	if (carry) {
		fp_addn_low(c, c, fp_prime_get());
	}
}

void fp_neg_basic(fp_t c, const fp_t a) {
	if (fp_is_zero(a)) {
		fp_zero(c);
	} else {
		fp_subn_low(c, fp_prime_get(), a);
	}
}

void fp_dbl_basic(fp_t c, const fp_t a) {
	dig_t carry;

	carry = fp_lsh1_low(c, a);
	if (carry || (dv_cmp(c, fp_prime_get(), RLC_FP_DIGS) != RLC_LT)) {
		carry = fp_subn_low(c, c, fp_prime_get());
	}
}

void fp_mul_basic(fp_t c, const fp_t a, const fp_t b) {
	int i;
	dv_t t;
	dig_t carry;

	/* We need a temporary variable so that c can be a or b. */
	dv_zero(t, 2 * RLC_FP_DIGS);
	for (i = 0; i < RLC_FP_DIGS; i++) {
		carry = fp_mula_low(t + i, b, *(a + i));
		*(t + i + RLC_FP_DIGS) = carry;
	}
	fp_rdc(c, t);
}

// reduction 取模运算 c = a%p
void fp_rdc_basic(fp_t c, dv_t a) {
	dv_t t0, t1, t2, t3;
	dv_copy(t2, a, 2 * RLC_FP_DIGS);
	dv_copy(t3, fp_prime_get(), RLC_FP_DIGS);
	bn_divn_low(t0, t1, t2, 2 * RLC_FP_DIGS, t3, RLC_FP_DIGS);
	fp_copy(c, t1);
}

void fp_print(fp_t a){
	for (int i = RLC_FP_DIGS-1; i >= 0; --i){
		printf("%016llx ", a[i]);
	}
	printf("\n");
}

void fp_copy(fp_t c, const fp_t a) {
	dv_copy(c, a, RLC_FP_DIGS);
}