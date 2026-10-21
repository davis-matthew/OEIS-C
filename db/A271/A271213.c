/*
 * A271213  a(n) = 2^(n-2) * (n! + floor(n/2)!).
 * https://oeis.org/A271213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * (n! + floor(n/2)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the base cases for the offset 0 */
    if (n == 0 || n == 1)
        return (seq_t)1;

    idx_t half = n / 2;          /* floor(n/2) */
    seq_t fact_n   = 1;          /* n!   */
    seq_t fact_half = 1;         /* floor(n/2)! */

    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
        if (i == half)
            fact_half = fact_n;
    }

    seq_t sum = fact_n + fact_half;
    seq_t pow = pow2(n - 2);     /* 2^(n-2) */

    return pow * sum;
}
