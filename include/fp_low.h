#ifndef FP_LOW_H
#define FP_LOW_H

#include "core.h"

dig_t fp_addn_low(dig_t *c, const dig_t *a, const dig_t *b);
dig_t fp_subn_low(dig_t *c, const dig_t *a, const dig_t *b);
dig_t fp_mula_low(dig_t *c, const dig_t *a, dig_t digit);
dig_t fp_lsh1_low(dig_t *c, const dig_t *a);

#endif