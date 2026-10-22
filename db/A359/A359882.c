/*
 * A359882  a(n) = Sum_{d|n} d^n * (n/d)^d.
 * https://oeis.org/A359882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^n * (n/d)^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 1.
   For small types this may overflow for n large enough; the value is
   correct for all terms that fit in seq_t, which is all terms up to n=15
   for a 64‑bit build.  The implementation uses only integer arithmetic
   and works for arbitrary-precision seq_t as well. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* outside the specified offset */

    seq_t total = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        /* Term for divisor d: d^n * (n/d)^d */
        seq_t term = 1;
        for (idx_t i = 0; i < n; ++i) term *= d;          /* d^n */

        seq_t part = 1;
        idx_t nd = n / d;
        for (idx_t i = 0; i < d; ++i) part *= nd;         /* (n/d)^d */
        term *= part;

        total += term;

        /* Handle the paired divisor nd if distinct */
        if (d != nd)
        {
            seq_t term2 = 1;
            for (idx_t i = 0; i < n; ++i) term2 *= nd;     /* nd^n */

            seq_t part2 = 1;
            for (idx_t i = 0; i < nd; ++i) part2 *= d;    /* d^nd */
            term2 *= part2;

            total += term2;
        }
    }

    return total;
}
