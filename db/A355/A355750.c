/*
 * A355750  Sum of the divisors of 2n minus the number of divisors of 2n.
 * https://oeis.org/A355750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2n) - tau(2n)
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of 2n minus the number of divisors of 2n.
   Implementation uses 128‑bit arithmetic for the intermediate value 2n
   to avoid overflow of uint64_t, and performs the divisor loop over
   uint64_t values. The results are accumulated in 64‑bit integers
   before being cast to seq_t. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2n as an unsigned 128‑bit value (avoids overflow when n is close to UINT64_MAX). */
    unsigned __int128 two_n = (unsigned __int128)n * 2ULL;

    unsigned long long sigma = 0ULL;   /* sum of divisors of 2n */
    unsigned long long tau   = 0ULL;   /* number of divisors of 2n */

    /* iterate over potential divisors i up to sqrt(2n) */
    for (idx_t i = 1; (unsigned __int128)i * i <= two_n; ++i)
    {
        if (two_n % i == 0)
        {
            /* i is a divisor */
            sigma += (unsigned long long)i;
            ++tau;

            /* compute the complementary divisor d2 = 2n / i */
            unsigned __int128 d2 = two_n / i;
            if (d2 != i)
            {
                /* d2 fits into unsigned long long for n up to 10^18,
                   which is more than enough for the intended test range. */
                sigma += (unsigned long long)d2;
                ++tau;
            }
        }
    }

    return (seq_t)(sigma - tau);
}
