/*
 * A368445  Expansion of e.g.f. exp(x) / (1 + log(1 - 3*x)).
 * https://oeis.org/A368445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} 3^k · (k‑1)! · binomial(n,k) · a(n‑k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368445 – expansion of exp(x)/(1 + log(1‑3x)) */

#include <stddef.h>
#include <stdlib.h>

/* External types; provided by the build system */
extern seq_t a(idx_t n) ;       /* ← we define below */

/* Internal memoisation */
static seq_t *cache   = NULL;   /* array holding computed a(i) */
static idx_t cap     = 0;       /* allocated size of cache          */
static idx_t len     = 0;       /* number of computed entries       */

seq_t a(idx_t n)
{
    /* Grow the cache if needed */
    if (n + 1 > cap) {
        idx_t newcap = (cap * 2 + 10);
        if (newcap < n + 1) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* cannot allocate more memory */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms in increasing order (not assuming
       that calls arrive monotonically in n).       */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum      = 0;        /* Σ term_k                           */
        seq_t C        = (seq_t)i; /* binomial(i,1)                      */
        seq_t pow3     = 3;        /* 3^1                                */
        seq_t factk_1  = 1;        /* (k‑1)! for k = 1  → 0! = 1         */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = cache[i - k];
            term *= C;          /* multiply by binomial(i,k)          */
            term *= pow3;       /* multiply by 3^k                    */
            term *= factk_1;    /* multiply by (k‑1)!                 */
            sum += term;

            if (k < i) {
                /* Prepare next values for k+1 */
                C        = (C * (idx_t)(i - k)) / (k + 1);
                pow3     = pow3 * 3;
                factk_1  = factk_1 * k;
            }
        }
        cache[i] = 1 + sum;       /* a(i) = 1 + Σ term_k               */
    }
    len = n + 1;
    return cache[n];
}
