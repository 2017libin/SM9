#ifndef CORE_H
#define CORE_H

#include <stdint.h>

/*============================================================================*/
/* 1. 常量定义                                                                 */
/*============================================================================*/
#define RLC_OK			0
#define RLC_ERR			1
#define RLC_LT			-1
#define RLC_EQ			0
#define RLC_GT			1
#define RLC_NE			2
#define RLC_ZERO		0
#define WSIZE           64
#define RLC_DIG	        64  // 每个精度的长度

#define RLC_MUL_DIG(H, L, A, B)												\
	H = ((dbl_t)(A) * (dbl_t)(B)) >> RLC_DIG;								\
	L = (A) * (B);															\

#define RLC_MASK(B)															\
	((-(uint64_t)((B) >= WSIZE)) | (((uint64_t)1 << ((B) % WSIZE)) - 1))

#define RLC_DIV_DIG(Q, R, H, L, D)											\
	Q = (((dbl_t)(H) << RLC_DIG) | (L)) / (D);								\
	R = (((dbl_t)(H) << RLC_DIG) | (L)) - (dbl_t)(Q) * (dbl_t)(D);			\

/*============================================================================*/
/* 2. 类型声明/别名                                                                    */
/*============================================================================*/
typedef uint64_t dig_t;
typedef __uint128_t dbl_t;

/*============================================================================*/
/* 3. 曲线参数                                                                  */
/*============================================================================*/
static const dig_t SM9_P[] = {0xe56f9b27e351457d, 0x21f2934b1a7aeedb, 0xd603ab4ff58ec745, 0xb640000002a3a6f1};
#endif