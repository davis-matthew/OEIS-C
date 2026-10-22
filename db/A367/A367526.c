/*
 * A367526  The number of ways of tiling the n X n grid up to diagonal and
 *          antidiagonal reflections by two tiles that are each fixed under
 *          both of these reflections.
 * https://oeis.org/A367526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m-1) = 2^(2m^2 - 4m - 1)*(4^m + 4^(m^2) + 8^m) a(2m) = 4^(m^2
 *            - 1)*(1 + 2^(1+m) + 4^(m^2))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) { /* even: n = 2m */
        idx_t m = n >> 1;          /* m = n/2 */
        idx_t mm = m * m;          /* m^2 */
        seq_t part1 = pow4(mm - 1);/* 4^(m^2-1) */
        seq_t part2 = pow2(1 + m); /* 2^(1+m)   */
        seq_t part3 = pow4(mm);    /* 4^(m^2)   */
        return part1 * (((seq_t)1) + part2 + part3);
    } else {                /* odd: n = 2m-1 */
        idx_t m  = (n + 1) >> 1;         /* m = (n+1)/2 */
        idx_t mm = m * m;                /* m^2 */
        /* signed exponent: 2*m^2 - 4*m - 1 */
        int64_t exp_signed = (int64_t)2 * mm - (int64_t)4 * m - 1;
        seq_t sum = pow4(m) + pow4(mm) + pow2(3 * m); /* 4^m + 4^(m^2) + 8^m */
        if (exp_signed >= 0)
            return pow2((idx_t)exp_signed) * sum;     /* 2^exp * sum */
        else {
            seq_t denom = pow2((idx_t)(-exp_signed));  /* 2^|exp| */
            return sum / denom;                       /* sum / 2^|exp| */
        }
    }
}
