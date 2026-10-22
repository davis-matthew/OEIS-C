/*
 * A367098  Number of divisors of n with exactly two distinct prime factors.
 * https://oeis.org/A367098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i<j} e_i · e_j where e_i are the exponents of distinct
 *            prime factors of n. (Number of divisors of n that have exactly
 *            two distinct prime factors.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert the index to a native integer for factorization.
       The function may be called with any n >= 1. */
    uint64_t x = (uint64_t)n;
    uint64_t exponents[64];
    int exp_cnt = 0;

    /* Trial division to factor n.  For n up to ~10^12 this is
       inexpensive; for the test constraints (n up to ~10000)
       it is trivial. */
    for (uint64_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            uint64_t e = 0;
            while (x % p == 0)
            {
                x /= p;
                e++;
            }
            exponents[exp_cnt++] = e;
        }
    }

    /* If n has a remaining prime factor larger than sqrt(original n). */
    if (x > 1)
    {
        exponents[exp_cnt++] = 1;
    }

    /* Compute Σ_{i<j} e_i * e_j. */
    seq_t total = 0;
    for (int i = 0; i < exp_cnt; ++i)
        for (int j = i + 1; j < exp_cnt; ++j)
            total += (seq_t)exponents[i] * (seq_t)exponents[j];

    return total;
}
