/*
 * A291013  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S^2)^3.
 * https://oeis.org/A291013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291013
 * Formula: a(n) = 2^(n-6) * (60 + 17*n + n^2)   for n > 2
 * Base values: a(0) = 0, a(1) = 3, a(2) = 6
 * Recurrence (n ≥ 7): a(n) = 6*a(n‑1) - 12*a(n‑2) + 8*a(n‑3)
 *   (not needed when using the closed formula)
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper: compute 2^exp; exp is non‑negative and fits in idx_t */
#ifdef __cplusplus
#include "../utils/pow2.h"   /* assumes pow2(idx_t) exists */
#else
/* In C, pow2 may be provided; if not, a simple loop works */
static seq_t my_pow2(idx_t exp)
{
    seq_t r = 1;
    while (exp--)
        r <<= 1;
    return r;
}
#define pow2 my_pow2
#endif

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    /* General formula for n > 2 */
    seq_t n_t      = (seq_t)n;
    seq_t num      = (seq_t)60 + (seq_t)17 * n_t + n_t * n_t;   /* (60+17n+n^2) */

    if (n >= 6) {                     /* 2^(n-6) * num  */
        seq_t pow10 = pow2(n - 6);
        return pow10 * num;
    } else {                          /* 0 < n < 6 : divide by 2^(6-n) */
        idx_t shift = 6 - n;          /* shift in [1..3] */
        return num >> shift;         /* exact division by powers of 2 */
    }
}
