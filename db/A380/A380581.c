/*
 * A380581  a(n) = [x^n] G(x)^n, where G(x) = Product_{k >= 1} 1/(1 -
 *          x^k)^(k^4) is the g.f. of A023873.
 * https://oeis.org/A380581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] G(x)^n, G(x) = ∏_{k≥1} 1/(1-x^k)^{k^4}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 22,
 *            big any n
 * Helpers:   to_sz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the identities
   G(x) = exp(∑_{k≥1} σ₅(k)/k · x^k)   (σ₅(k)=∑_{d|k} d⁵)
   G^n(x) = exp(n·∑_{k≥1} σ₅(k)/k · x^k)
   The coefficients of G and its powers are obtained by successive
   convolution. This implementation runs in O(n²) time for
   a single query and produces the exact value for any n. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/to_sz.h"

/* main function --------------------------------------------------- */
seq_t a(idx_t n)
{
    size_t N = to_sz(n);                         /* target index   */
    if (N == 0) return 1;                       /* a(0) = 1       */

    /* ----------------------------------------------------------------- */
    /* 1. compute the divisor sums σ₅(k) for k = 1 .. N          */
    /* ----------------------------------------------------------------- */
    seq_t *sigma5 = (seq_t*)calloc(N + 1, sizeof(seq_t));
    for (size_t d = 1; d <= N; ++d) {
        seq_t d_pow5 = (seq_t)d;
        d_pow5 = d_pow5 * (seq_t)d;   /* d² */
        d_pow5 = d_pow5 * (seq_t)d;   /* d³ */
        d_pow5 = d_pow5 * (seq_t)d;   /* d⁴ */
        d_pow5 = d_pow5 * (seq_t)d;   /* d⁵ */
        for (size_t m = d; m <= N; m += d) {
            sigma5[m] += d_pow5;
        }
    }

    /* ----------------------------------------------------------------- */
    /* 2. compute the coefficients g[0..N] of G(x) = exp(∑ σ₅(k)/k x^k) */
    /* ----------------------------------------------------------------- */
    seq_t *g = (seq_t*)calloc(N + 1, sizeof(seq_t));
    g[0] = 1;                                     /* G(0) = 1          */
    for (size_t m = 1; m <= N; ++m) {
        seq_t sum = 0;
        for (size_t k = 1; k <= m; ++k) {
            sum += sigma5[k] * g[m - k];
        }
        g[m] = sum / (seq_t)m;                    /* division by m   */
    }

    /* ----------------------------------------------------------------- */
    /* 3. compute G(x)^N by successive convolution                */
    /*    We keep two temporary arrays: cur and next                */
    /* ----------------------------------------------------------------- */
    seq_t *cur  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(N + 1, sizeof(seq_t));

    cur[0] = 1; /* start with the empty product (G^0)   */

    for (size_t step = 1; step <= N; ++step) {
        /* compute next[i] = Σ_{k=0}^{i} cur[k] * g[i-k]  */
        for (size_t i = 0; i <= N; ++i) {
            seq_t sum = 0;
            for (size_t k = 0; k <= i; ++k) {
                sum += cur[k] * g[i - k];
            }
            next[i] = sum;
        }
        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t answer = cur[N];

    /* ----------------------------------------------------------------- */
    /* 4. cleanup                                            */
    /* ----------------------------------------------------------------- */
    free(sigma5);
    free(g);
    free(cur);
    free(next);

    return answer;
}
