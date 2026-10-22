/*
 * A385438  a(n) = Sum_{k=0..n} 3^k * binomial(3*n+k-1,k).
 * https://oeis.org/A385438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(3*n + k - 1, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t definition used by idx_t */

seq_t a(idx_t n)
{
    /* Handle n = 0 automatically – the loop below works for all n. */
    seq_t ans = 0;
    seq_t B   = 1;                 /* binomial(3*n + 0 - 1, 0) = 1            */
    seq_t pow3 = 1;                 /* 3^0                                        */
    seq_t three_n = (seq_t)(3 * n); /* 3*n, cast to seq_t for later use        */

    for (idx_t k = 0; k <= n; ++k) {
        /* Current term: 3^k * binom(3*n + k - 1, k) */
        seq_t term = B * pow3;
        ans += term;

        if (k < n) {
            /* Update B to binom(3*n + (k+1) - 1, k+1) */
            B = (B * (three_n + k)) / (k + 1);
            /* Update 3^k to 3^(k+1) */
            pow3 *= 3;
        }
    }
    return ans;
}
