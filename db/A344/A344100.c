/*
 * A344100  Expansion of Product_{k>=1} (1 + x^k)^binomial(k+4,5).
 * https://oeis.org/A344100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the generating function G(x) = ∏_{k≥1} (1 +
 *            x^k)^{C(k+4,5)}. Let f_k be the coefficients of log G(x): f_k =
 *            Σ_{d|k} (-1)^{d+1} * C(k/d + 4,5) / d Then set g_k = k * f_k =
 *            Σ_{d|k} (-1)^{d+1} * C(k/d + 4,5) * (k/d). The coefficients a(n)
 *            satisfy a(0) = 1, a(n) = (1/n) * Σ_{k=1}^{...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *A = NULL;   /* a[0 .. max_n]               */
    static seq_t *G = NULL;   /* g[0 .. max_n]   (g[0] unused) */
    static idx_t max_n = 0;   /* largest n for which A/G filled */

    /* Initialise cache for n = 0 */
    if (max_n == 0) {
        A = (seq_t *)malloc(sizeof(seq_t));
        G = (seq_t *)malloc(sizeof(seq_t));
        assert(A && G);
        A[0] = (seq_t)1;      /* a(0) = 1 */
        G[0] = (seq_t)0;      /* unused */
        max_n = 0;
    }

    if (n <= max_n)
        return A[n];

    /* Extend the cache up to n */
    seq_t *A_new = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));
    seq_t *G_new = (seq_t *)realloc(G, (n + 1) * sizeof(seq_t));
    assert(A_new && G_new);
    A = A_new;
    G = G_new;

    /* Compute g[k] for k = max_n+1 .. n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* iterate over divisors of k */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d != 0)
                continue;
            /* divisor d */
            idx_t m = k / d;  /* m = k/d */
            seq_t bin = binomial(m + 4, 5);          /* C(m+4,5) */
            seq_t term = seq_mul(bin, (seq_t)m);     /* term = bin * m */
            if (d & 1U)
                pos += term;
            else
                neg += term;

            /* paired divisor k/d if distinct */
            if (d * d != k) {
                idx_t d2 = k / d;  /* other divisor */
                idx_t m2 = k / d2; /* equals d */
                seq_t bin2 = binomial(m2 + 4, 5);
                seq_t term2 = seq_mul(bin2, (seq_t)m2);
                if (d2 & 1U)
                    pos += term2;
                else
                    neg += term2;
            }
        }
        /* g_k = pos - neg  (pos >= neg by construction) */
        seq_t gk = pos;
        gk -= neg;
        G[k] = gk;
    }

    /* Compute a[n] for n = max_n+1 .. n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = seq_mul(G[k], A[i - k]);
            sum += prod;
        }
        A[i] = sum / (seq_t)i;   /* exact division */
    }

    max_n = n;
    return A[n];
}
