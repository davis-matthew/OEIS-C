/*
 * A385497  a(n) = Sum_{k=0..n} binomial(6*n+1,k).
 * https://oeis.org/A385497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(6*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* Accumulate the sum.   */
    seq_t bin = 1;          /* binomial coefficient. */

    /* Compute the upper index of the binomial: 6*n+1.  */
    seq_t m = (seq_t)6 * (seq_t)n + 1;

    for (idx_t k = 0; k <= n; ++k) {
        /* Add the current binomial term. */
        sum += bin;

        /* No more terms after the last one. */
        if (k == n) break;

        /* Prepare the next binomial coefficient:
           bin_{k+1} = bin_k * (m - k) / (k + 1).           */
        seq_t num = m - (seq_t)k;         /* numerator     */
        seq_t den = (seq_t)(k + 1);       /* denominator   */

        /* Reduce the fraction naively to avoid overflow. */
        seq_t g1 = gcd(den, num);
        if (g1 > 1) {                    /* divide num/den */
            num /= g1;
            den /= g1;
        }

        seq_t g2 = gcd(den, bin);
        if (g2 > 1) {                    /* divide den/bin */
            den /= g2;
            bin /= g2;
        }

        /* After reductions, den should be 1.  Multiply safely. */
        bin = bin * num;
    }

    return sum;
}
