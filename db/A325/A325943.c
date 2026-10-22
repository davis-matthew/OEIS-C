/*
 * A325943  a(n) = floor(n / omega(n)) where omega = A001221.
 * https://oeis.org/A325943
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / omega(n)) where omega(n) is the number of
 *            distinct prime factors
 * Range:     u8 n <= 255, u16 n <= 9856, u32 n <= 9856, u64 n <= 9856,
 *            u128 n <= 9856, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: count distinct prime factors of an unsigned integer using trial division. */
static int count_distinct_primes(idx_t n)
{
    int cnt = 0;
    idx_t x = n;
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            ++cnt;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* behavior below offset is unspecified */

    /* Count distinct prime factors (omega). */
    int omega = count_distinct_primes(n);

    /* Omega(n) >= 1 for n >= 2, so division is safe. */
    return (seq_t)n / (seq_t)omega;
}
