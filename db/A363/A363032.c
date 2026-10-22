/*
 * A363032  Expansion of Sum_{k>0} k * x^(3*k-1) / (1 - x^(5*k-3)).
 * https://oeis.org/A363032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | (5*n-4), d==2 mod 5} (d+3)
 * Range:     u8 n <= 51, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A363032.  For n < 1 the function
   returns 0, which is ignored by the caller. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t m   = (seq_t)5 * n - 4;   /* m = 5*n - 4   */
    seq_t sum = 0;

    /* Compute floor(sqrt(m)) without floating‑point arithmetic. */
    idx_t limit = 0;
    while ((seq_t)(limit + 1) * (limit + 1) <= m)
        ++limit;

    for (idx_t d = 1; d <= limit; ++d) {
        if (m % d == 0) {
            seq_t other = m / d;

            /* First divisor d. */
            if (d % 5 == 2)
                sum += (seq_t)d + 3;

            /* Second divisor m/d, if distinct. */
            if (other != d && other % 5 == 2)
                sum += other + 3;
        }
    }

    return sum / 5;
}
