/*
 * A257633  a(n) = binomial(4*n + 2,n).
 * https://oeis.org/A257633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(4*n + 2, n)
 *
 * The binomial coefficient is computed multiplicatively, reducing at each
 * step with gcd to avoid overflow of intermediate results when seq_t is a
 * fixed‑size integer type (e.g. uint64_t).  For arbitrary precision,
 * the same algorithm works and gives the exact result.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Fast return for the first term */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* N = 4*n + 2 is the upper argument of the binomial */
    seq_t N = (seq_t)4 * (seq_t)n + (seq_t)2;

    for (idx_t k = 1; k <= n; ++k) {
        /* numerator = N - n + k = 3*n + 2 + k */
        seq_t numerator   = N - (seq_t)n + (seq_t)k;
        seq_t denominator = (seq_t)k;

        /* Reduce numerator and denominator by their gcd */
        seq_t g = gcd(numerator, denominator);
        numerator   /= g;
        denominator /= g;

        /* Reduce result and remaining denominator by gcd */
        g = gcd(result, denominator);
        result   /= g;
        denominator /= g;   /* should now be 1 */

        /* Multiply and continue */
        result *= numerator;
    }

    return result;
}
