/*
 * A359190  a(n) = Sum_{d|n} d * 4^(n/d-1).
 * https://oeis.org/A359190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * 4^(n/d-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* 4^(e) by exponentiation by squaring.  e fits in idx_t. */
static inline seq_t pow4(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    for (idx_t x = e; x > 0; x >>= 1)
    {
        if (x & 1) result = result * base;
        base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* behavior below the offset is irrelevant */

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d)   /* iterate over divisors up to sqrt(n) */
    {
        if (n % d == 0)
        {
            idx_t k1 = n / d;          /* paired divisor */
            sum += (seq_t)d * pow4(k1 - 1);
            if (d != k1)
                sum += (seq_t)k1 * pow4(d - 1);
        }
    }
    return sum;
}
