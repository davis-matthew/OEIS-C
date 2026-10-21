/*
 * A274271  Number of partitions of 3^n into at most four parts.
 * https://oeis.org/A274271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3 + 3^n)^2 * (9 + 3^n)) / 144 for n > 1 Special cases:
 *            a(0) = 1, a(1) = 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* For n > 1 we use the simplified formula
         a(n) = (3 * (3^(n-1) + 1)^2 * (3^(n-1) + 3)) / 16
       which avoids large intermediate products before the final division. */
    seq_t r = pow3(n - 1);          /* r = 3^(n-1) */
    seq_t s = r + (seq_t)1;        /* s = r + 1   */
    seq_t t = r + (seq_t)3;        /* t = r + 3   */

    seq_t result = s * s;          /* s^2          */
    result *= t;                   /* s^2 * t      */
    result *= (seq_t)3;            /* 3 * s^2 * t  */
    result /= (seq_t)16;           /* divide by 16 */

    return result;
}
