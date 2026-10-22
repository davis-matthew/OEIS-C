/*
 * A365129  G.f. A(x) satisfies A(x) = (1 + x*A(x)*(1 + x*A(x))^2)^2.
 * https://oeis.org/A365129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} binomial(2*(n+1), k) *
 *            binomial(2*k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) is available */

seq_t a(idx_t n) {
    /* Convert index to seq_t to avoid mixed‐type issues */
    seq_t N   = (seq_t)n + 1;          /* n+1 */
    seq_t twot = (seq_t)2 * N;         /* 2*(n+1) */

    seq_t sum  = 0;                    /* running sum of terms */
    seq_t bin1 = 1;                    /* binomial(twot, 0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(2*k, n-k) */
        seq_t bin2 = binomial((idx_t)(2 * k), (idx_t)(n - k));

        /* Add current term to sum */
        sum += bin1 * bin2;

        /* Update bin1 to binomial(twot, k+1) for next iteration */
        if (k < n) {
            seq_t numerator = twot - (seq_t)k;     /* 2*(n+1) - k */
            bin1 = bin1 * numerator / ((seq_t)k + 1);
        }
    }

    /* Divide by (n+1) */
    return sum / N;
}
