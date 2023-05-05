#ifndef DV_H
#define DV_H

#include "core.h"

typedef dig_t dv_t[34];

int dv_cmp(const dig_t *a, const dig_t *b, int size);
void dv_zero(dig_t *a, int digits);
void dv_lshd(dig_t *c, const dig_t *a, int size, int digits);
void dv_rshd(dig_t *c, const dig_t *a, int size, int digits);
void dv_copy(dig_t *c, const dig_t *a, int digits);
#endif