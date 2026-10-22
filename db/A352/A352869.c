/*
 * A352869  Expansion of e.g.f. 1/(1 - Sum_{k>=1} mu(k) * x^k/k!), where mu()
 *          is the Moebius function (A008683).
 * https://oeis.org/A352869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A352869
 *  Recurrence:  a(0) = 1
 *                a(n) = Σ_{k=1..n} μ(k) * C(n,k) * a(n‑k)   (n ≥ 1)
 *  μ(k) is the Möbius function (A008683).  The implementation caches
 *  previously computed a(n) values and μ(k) values, so calls may be made
 *  in any order.  All arithmetic is performed in seq_t; no floating‑
 *  point is used.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* ---------- Cache for a(n) values ---------- */
    static seq_t *cache      = NULL;   /* a[0..cache_size-1]   */
    static idx_t cache_size  = 0;      /* number of cached items*/

    /* ---------- Cache for μ(k) values (int) ---------- */
    static int  *mu          = NULL;   /* mu[1..mu_size-1]     */
    static idx_t mu_size     = 0;      /* number of mu values  */

    /* ---- Ensure cache can hold a(0..n) ---- */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failed */
        cache = new_cache;
        /* initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* ---- Ensure μ(1..n) are known ---- */
    if (n >= mu_size) {
        idx_t old_mu_size = mu_size;
        idx_t new_mu_size = n + 1;                 /* allow index n */
        int *new_mu = (int *)realloc(mu, new_mu_size * sizeof(int));
        if (new_mu == NULL) return (seq_t)0;       /* allocation failed */
        mu = new_mu;

        /* For new entries, compute μ using a small‑prime‑factor sieve */
        idx_t *spf = NULL;
        build_spf(n, &spf);                /* gives spf[0..n] */

        if (new_mu_size > 0) new_mu[0] = 0;   /* unused, but defined */
        if (new_mu_size > 1) new_mu[1] = 1;   /* μ(1)=1 */

        for (idx_t i = 2; i <= n; ++i) {
            idx_t x = i;
            int   sign = 1;
            int   squarefree = 1;
            while (x > 1) {
                idx_t p = spf[x];
                int   cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                if (cnt > 1) {               /* not square‑free */
                    squarefree = 0;
                    break;
                }
                sign = -sign;
            }
            new_mu[i] = squarefree ? sign : 0;
        }
        free(spf);
        mu_size = new_mu_size;
    }

    /* ---- Compute a(cur) for all new indices ---- */
    for (idx_t cur = 0; cur <= n; ++cur) {
        if (cur == 0) {          /* base case */
            cache[0] = 1;
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = 1;         /* C(cur,0) */

        for (idx_t k = 1; k <= cur; ++k) {
            /* Update binomial coefficient C(cur,k) */
            binom = (binom * (seq_t)(cur - k + 1)) / (seq_t)k;
            int mk = mu[k];
            if (mk == 0) continue;                /* skip zero terms */
            seq_t term = binom * cache[cur - k];
            if (mk == -1) term = -term;           /* μ(k) = -1 */
            sum += term;
        }
        cache[cur] = sum;
    }

    return cache[n];
}
