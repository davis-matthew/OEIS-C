/*
 * A370751  a(n) = 2^n * [x^n] Product_{k>=1} ((1 + 3*x^k)/(1 - 3*x^k))^(1/2).
 * https://oeis.org/A370751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * [x^n] sqrt( ∏_{k≥1} ((1 + 3*x^k)/(1 - 3*x^k)) )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate working arrays */
    idx_t N = n;
    seq_t *h    = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *B    = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *pow3 = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *ainc = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!h || !B || !pow3 || !ainc) {
        free(h); free(B); free(pow3); free(ainc);
        return (seq_t)0;
    }

    /* Pre‑compute powers of 3 : pow3[d] = 3^d  (d = 0 … N) */
    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        pow3[i] = pow3[i-1] * (seq_t)3;
    }

    /* Compute B[k] = 2 * Σ_{d|k, d odd} (k/d) * 3^d  (integer) */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                if (d & 1) {
                    idx_t factor = k / d;
                    sum += pow3[d] * (seq_t)factor;
                }
                idx_t d2 = k / d;
                if (d2 != d && (d2 & 1)) {
                    idx_t factor = k / d2;
                    sum += pow3[d2] * (seq_t)factor;
                }
            }
        }
        B[k] = sum * (seq_t)2;
    }

    /* Compute h[n] : coefficients of the product ∏(1 + 2∑_{m≥1} 3^m x^{k m})  */
    h[0] = (seq_t)1;
    for (idx_t n = 1; n <= N; ++n) {
        seq_t tmp = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            tmp += B[k] * h[n - k];
        }
        h[n] = tmp / (seq_t)n;          /* division is exact */
    }

    /* Compute a[n] via a[n] = (2^n*h[n] - Σ_{k=1}^{n-1} a[k]a[n-k]) / 2  */
    ainc[0] = (seq_t)1;
    seq_t pow2 = (seq_t)1;               /* 2^0 */
    for (idx_t i = 1; i <= N; ++i) {
        pow2 *= (seq_t)2;                /* 2^i */
        seq_t left = h[i] * pow2;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            sum += ainc[k] * ainc[i - k];
        }
        ainc[i] = (left - sum) / (seq_t)2;
    }

    seq_t result = ainc[N];

    free(h); free(B); free(pow3); free(ainc);
    return result;
}
