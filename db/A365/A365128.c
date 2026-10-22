/*
 * A365128  G.f. A(x) satisfies A(x) = (1 + x*A(x)*(1 + x*A(x)))^3.
 * https://oeis.org/A365128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} binomial(3*(n+1), k) *
 *            binomial(k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Common parameters */
    const idx_t N = 3 * (n + 1);

    /* Computation depends on the size of seq_t.
       For 64‑bit (or smaller) seq_t we use unsigned __int128
       for intermediate products to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate arithmetic */
        unsigned __int128 sum   = 0;
        unsigned __int128 binN  = 1;   /* binomial(N,0) */

        for (idx_t k = 0; k <= n; ++k) {
            /* binomial(k, n-k) is non‑zero only for k >= n-k */
            if (k >= n - k) {
                /* Compute binom(k, n-k) using multiplicative formula */
                unsigned __int128 bin2 = 1;
                const idx_t r = n - k;          /* r = n-k <= k */
                for (idx_t i = 1; i <= r; ++i) {
                    bin2 = bin2 * (k - r + i) / i;
                }
                sum += binN * bin2;
            }

            /* Update binN to binomial(N, k+1) for the next iteration */
            if (k < n) {
                unsigned __int128 mult = N - k;
                unsigned __int128 div  = k + 1;
                binN = binN * mult / div;
            }
        }

        return (seq_t)(sum / (unsigned __int128)(n + 1));
    }
    /* For larger seq_t (arbitrary precision), use the binom() helper
       and normal integer arithmetic (now relying on the arbitrary
       precision implementation inside binom for overhead). */
    else {
        seq_t sum  = 0;
        seq_t binN = 1;   /* binomial(N,0) */

        for (idx_t k = 0; k <= n; ++k) {
            if (k >= n - k) {
                seq_t bin2 = binom(k, n - k);
                sum += binN * bin2;
            }
            if (k < n) {
                binN = binN * (seq_t)(N - k) / (seq_t)(k + 1);
            }
        }

        return sum / (seq_t)(n + 1);
    }
}
