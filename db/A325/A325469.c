/*
 * A325469  a(n) is the number of divisors d of n such that d divides
 *          sigma(d).
 * https://oeis.org/A325469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d divides σ(d)
 * Range:     u8 n <= 9818, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9818 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided sigma_of helper to compute σ(d),
   iterates over all divisors of n, and counts those for which σ(d) % d == 0. */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Enumerate divisors using integer square root to avoid overflow */
    idx_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            seq_t sigma_i = sigma_of(i);
            if (sigma_i % (seq_t)i == 0)
                ++count;

            /* counterpart divisor n / i, if distinct */
            idx_t j = n / i;
            if (j != i)
            {
                seq_t sigma_j = sigma_of(j);
                if (sigma_j % (seq_t)j == 0)
                    ++count;
            }
        }
    }
    return count;
}
