/*
 * A359018  a(n) = Sum_{d|n} d * 3^(d-1).
 * https://oeis.org/A359018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * 3^(d-1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

/* Compute 3^exp for exp >= 0 using binary exponentiation
   (exp is an unsigned 64‑bit integer). */
static seq_t pow_uint3(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (exp > 0)
    {
        if (exp & 1U)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U)  /* offset starts at 1; value for n <= 0 is irrelevant */
        return 0;

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t term1 = seq_mul((seq_t)d, pow_uint3(d - 1));
            result += term1;

            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t term2 = seq_mul((seq_t)d2, pow_uint3(d2 - 1));
                result += term2;
            }
        }
    }
    return result;
}
