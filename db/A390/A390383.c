/*
 * A390383  a(n) = Sum_{k=0..n} binomial(n+3*k+3,k).
 * https://oeis.org/A390383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+3*k+3, k)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sum the binomial coefficients incrementally. */
    seq_t sum = 1;          /* a(0) = 1  */
    seq_t bin = 1;          /* current f_k, starting with k=0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute the three numerators and three denominators for the ratio */
        seq_t a1 = (seq_t)n + 3 * k + 1;
        seq_t a2 = (seq_t)n + 3 * k + 2;
        seq_t a3 = (seq_t)n + 3 * k + 3;
        seq_t d1 = (seq_t)k;
        seq_t d2 = (seq_t)n + 2 * k + 2;
        seq_t d3 = (seq_t)n + 2 * k + 3;

        seq_t numer = a1 * a2 * a3;
        seq_t denom = d1 * d2 * d3;

        /* Reduce the fraction numer/denom to keep intermediates smaller */
        seq_t g1 = gcd(numer, denom);
        numer /= g1;
        denom /= g1;

        /* Reduce the current bin coefficient with the remaining denominator */
        seq_t g2 = gcd(bin, denom);
        bin /= g2;
        denom /= g2;

        /* Apply the ratio to update bin */
        bin *= numer;
        bin /= denom;

        sum += bin;
    }
    return sum;
}
