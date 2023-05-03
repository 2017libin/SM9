#include "core.h"

int dv_cmp(const dig_t *a, const dig_t *b, int size) {
	int i, r;

	a += (size - 1);
	b += (size - 1);

	r = RLC_EQ;
	for (i = 0; i < size; i++, --a, --b) {
		if (*a != *b && r == RLC_EQ) {
			r = (*a > *b ? RLC_GT : RLC_LT);
		}
	}
	return r;
}

void dv_zero(dig_t *a, int digits) {
	int i;

	for (i = 0; i < digits; i++, a++) {
		(*a) = 0;
	}

	return;
}

void dv_lshd(dig_t *c, const dig_t *a, int size, int digits) {
	dig_t *top;
	const dig_t *bot;
	int i;

	top = c + size - 1;
	bot = a + size - 1 - digits;

	for (i = 0; i < size - digits; i++, top--, bot--) {
		*top = *bot;
	}
	for (i = 0; i < digits; i++, c++) {
		*c = 0;
	}
}

void dv_rshd(dig_t *c, const dig_t *a, int size, int digits) {
	const dig_t *top;
	dig_t *bot;
	int i;

	top = a + digits;
	bot = c;

	for (i = 0; i < size - digits; i++, top++, bot++) {
		*bot = *top;
	}
	for (; i < size; i++, bot++) {
		*bot = 0;
	}
}

void dv_copy(dig_t *c, const dig_t *a, int digits) {
	memcpy(c, a, digits * sizeof(dig_t));
}