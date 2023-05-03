#ifndef FP_H
#define FP_H

#include "core.h"
/*============================================================================*/
/* 1. 常量定义                                                                    */
/*============================================================================*/
#define RLC_FP_DIGS 4
#define FP_PRIME SM9_P

/*============================================================================*/
/* 2. 类型声明/别名                                                                    */
/*============================================================================*/
typedef dig_t fp_t[RLC_FP_DIGS];

/*============================================================================*/
/* 3. 函数声明                                                                    */
/*============================================================================*/
void fp_add_basic(fp_t c, const fp_t a, const fp_t b);
void fp_sub_basic(fp_t c, const fp_t a, const fp_t b);
void fp_neg_basic(fp_t c, const fp_t a);
void fp_dbl_basic(fp_t c, const fp_t a);
void fp_mul_basic(fp_t c, const fp_t a, const fp_t b);

#define fp_rdc fp_rdc_basic
#endif
