/*
 * A248008  Least positive integer m such that m + n divides sigma(m*n), where
 *          sigma(k) denotes the sum of all positive divisors of k.
 * https://oeis.org/A248008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that (m + n) divides σ(m*n)
 * Range:     u8 n <= 9, u16 n <= 1308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where σ(k) is the sum of all positive divisors of k.                       */

#include <stdint.h>
#include <stdbool.h>
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
    /* n is guaranteed to be ≥ 1 by the sequence definition.                */
    idx_t m = 1;
    for (;; ++m)
    {
        /* Prevent overflow of the product m*n (unlikely for n ≤ 1e4).      */
        if (n != 0 && m > UINT64_MAX / n)
            return (seq_t)0;      /* never expected to reach here          */

        idx_t prod  = n * m;                       /* m * n                                 */
        seq_t sigma = sigma_of(prod);              /* σ(m*n)                                */
        seq_t mod   = (seq_t)(m + n);              /* divisor to test                        */

        if ((sigma % mod) == 0)
            return (seq_t)m;                        /* first m satisfying the condition      */
    }
}
