/*
 * A363646  Expansion of Sum_{k>0} (1/(1 - (k*x)^k)^2 - 1).
 * https://oeis.org/A363646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * (d+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:
 * The sequence grows extremely fast.  For n up to about 15 the result
 * fits into 64 bits; larger values are handled correctly only by the
 * arbitrary‑precision build.
 *
 * Implementation uses the utility function big_pow(base, exp) to
 * compute (n/d)^n.  Divisors are enumerated by a simple trial division
 * loop that runs in O(√n).  This is fast enough for n ≤ 10 000.
 */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t i;
    seq_t sum = 0;

    /* Iterate over all divisors d of n.  Since n ≤ 10 000, sqrt(n) < 100. */
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* First divisor d1 = i */
            {
                idx_t d = i;
                seq_t base = (seq_t)(n / d);
                seq_t power = big_pow(base, (seq_t)n);
                seq_t term = power * ((seq_t)(d + 1));
                sum += term;
            }

            /* Second divisor d2 = n / i, if distinct */
            if (i * i != n)
            {
                idx_t d = n / i;
                seq_t base = (seq_t)(n / d);
                seq_t power = big_pow(base, (seq_t)n);
                seq_t term = power * ((seq_t)(d + 1));
                sum += term;
            }
        }
    }

    return sum;
}
