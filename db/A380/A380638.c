/*
 * A380638  Expansion of e.g.f. exp(x*G(4*x)^4) where G(x) = 1 + x*G(x)^4 is
 *          the g.f. of A002293.
 * https://oeis.org/A380638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0}^{n-1} 4^k * binomial(4*n, k) / (n-k-1)!
 *            (for n > 0) a(0) = 1 The implementation evaluates the sum by
 *            iterating k from 0 to n-1, maintaining the falling factorial
 *            (n-1)(n-2)…(n-k), the binomial coefficient binomial(4*n, k), and
 *            4^k incrementally to keep the computation O(n)...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for uint64_t used by idx_t in the build */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    seq_t sum = 0;                 /* accumulator for the sum */
    seq_t prod_fall = 1;           /* falling factorial (n-1)(n-2)...(n-k)  */
    seq_t binom = 1;               /* binomial(4*n, k)   */
    seq_t pow4 = 1;                /* 4^k                */
    idx_t four_n = 4 * n;          /* 4*n as idx_t           */

    for (idx_t k = 0; k < n; ++k) {
        /* term = binom * prod_fall * pow4   */
        seq_t term = binom;
        term *= prod_fall;
        term *= pow4;
        sum += term;

        /* Update for next k, but not after the last iteration */
        if (k + 1 < n) {
            /* prod_fall *= (n-1 - k)  */
            prod_fall *= (seq_t)(n - 1 - k);

            /* binom = binom * (4*n - k) / (k+1)  */
            binom = (binom * (seq_t)(four_n - k)) / (seq_t)(k + 1);

            /* pow4 *= 4  */
            pow4 *= 4;
        }
    }
    return sum;
}
