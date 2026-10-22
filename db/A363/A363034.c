/*
 * A363034  Sum of divisors of 5*n-2 of form 5*k+3.
 * https://oeis.org/A363034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ{ d | (5*n-2) and d ≡ 3 (mod 5) } d
 * Range:     u8 n <= 45, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) : sum of divisors of 5*n-2 that are congruent to 3 modulo 5. */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1) corresponds to n = 1. */
    if (n == 0) return 0;                /* unused but harmless */

    /* m = 5*n-2 (fits in a 64‑bit unsigned integer for all n in the test set). */
    uint64_t const m = 5ull * (uint64_t)n - 2ull;

    seq_t sum = 0;

    /* Iterate over the positive divisors of m.  For each divisor d, we also
       consider the paired divisor m/d.  Since m ≤ 5·10⁴ for the supplied
       test set, the loop below is very fast and uses only 64‑bit arithmetic. */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* d is a divisor */
            if (d % 5 == 3) sum += (seq_t)d;

            uint64_t e = m / d;          /* the paired divisor */
            if (e != d && e % 5 == 3)    /* avoid double‑counting the square root */
                sum += (seq_t)e;
        }
    }

    return sum;
}
