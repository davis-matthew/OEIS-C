/*
 * A397078  Expansion of g.f. (Sum_{j>=1} x^j/(1-x^j))^2 * Product_{k>=1}
 *          (1+x^k).
 * https://oeis.org/A397078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ( Sum_{j≥1} x^j/(1–x^j) )^2 *
 *            Product_{k≥1} (1 + x^k)
 * Range:     u8 n <= 9, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *divcnt  = NULL;   /* divisor counts d(k) */
static seq_t *conv    = NULL;   /* convolution C(k) = Σ d(i)d(k-i) */
static seq_t *partd   = NULL;   /* partitions into distinct parts p(n) */
static idx_t  max_n   = 0;      /* highest n for which the arrays are valid */

/* Allocate / rebuild the cache up to n */
static void ensure_cache(idx_t n)
{
    if (n <= max_n) return;

    /* free old memory */
    if (divcnt) free(divcnt);
    if (conv)   free(conv);
    if (partd)  free(partd);

    size_t sz = (size_t)(n + 1);
    divcnt = (seq_t*)malloc(sz * sizeof(seq_t));
    conv   = (seq_t*)malloc(sz * sizeof(seq_t));
    partd  = (seq_t*)malloc(sz * sizeof(seq_t));

    /* zero‑initialize all arrays */
    for (idx_t i = 0; i <= n; ++i) {
        divcnt[i] = 0;
        conv[i]   = 0;
        partd[i]  = 0;
    }

    /* divisor function d(k) for 1 ≤ k ≤ n */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            divcnt[j] = divcnt[j] + 1;
        }
    }

    /* convolution C(k) = Σ_{i=1}^{k-1} d(i)d(k-i) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t di = divcnt[i];
        for (idx_t j = 1; j <= n - i; ++j) {
            conv[i + j] = conv[i + j] + di * divcnt[j];
        }
    }

    /* partitions into distinct parts via DP:
       p[0] = 1; for part in 1..n:
          for sum = n..part: p[sum] += p[sum-part]   */
    partd[0] = 1;
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            partd[sum] = partd[sum] + partd[sum - part];
        }
    }

    max_n = n;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        result = result + conv[k] * partd[n - k];
    }
    return result;
}
