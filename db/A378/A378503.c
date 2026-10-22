/*
 * A378503  Expansion of (Sum_{k>=0} binomial(4*k,k) * x^k)^3.
 * https://oeis.org/A378503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 3^k * binomial(4*n+2, n-k) (n>=0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t t = 4*n + 2;          /* 4*n + 2 used in every binomial call */
    seq_t result = 0;
    seq_t pow3 = 1;             /* 3^k, start with k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(t, n - k);   /* binomial(4*n+2, n-k) */
        term *= (seq_t)(k + 1);           /* multiply by (k+1) */
        term *= pow3;                     /* multiply by 3^k */
        result += term;

        pow3 *= 3;                        /* advance 3^(k+1) for next iteration */
    }
    return result;
}
