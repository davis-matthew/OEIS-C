/*
 * A383326  a(n) = Sum_{k=0..n} binomial(3*n+1,k) * binomial(3*n-k,n-k).
 * https://oeis.org/A383326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(2*n + k, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute OEIS sequence A383326: a(n) = Σ_{k=0..n} C(3n+1,k) * C(3n-k,n-k) */
seq_t a(idx_t n)
{
    seq_t sum = 0;          /* running sum of terms */
    seq_t bin = 1;          /* C(2n+0, 0) = 1 */
    seq_t pow2 = 1;         /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 2^k * C(2n + k, k) */
        sum += pow2 * bin;

        if (k == n) break; /* last iteration finished */

        /* update bin for next k:
         * bin = bin * (2n + k + 1) / (k + 1)
         * The division is exact because bin(k) is an integer binomial.
         */
        bin = bin * (seq_t)(2 * n + k + 1) / (k + 1);

        /* update power of two for next term */
        pow2 *= 2;
    }

    return sum;
}
