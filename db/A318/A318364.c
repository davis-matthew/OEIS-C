/*
 * A318364  Expansion of e.g.f. exp(x*exp(x)/(1 - x)).
 * https://oeis.org/A318364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A318364
   Coefficients of the e.g.f. exp(x*exp(x)/(1 - x))

   Recurrence:
      a(0) = 1
      a(n) = Σ_{k=1..n} A007526(k) * binomial(n-1, k-1) * a(n-k)   (n ≥ 1)

   where A007526(k) = k! * Σ_{m=0}^{k-1} 1/m! .
*/

#include <stdlib.h>

/* static cache for sequence values and pre‑computed A007526 values */
static seq_t *a_cache = NULL;
static seq_t *w_cache = NULL;
static size_t  cap   = 0;           /* allocated size (≥ max_n+1)       */
static idx_t   max_n = 0;           /* largest n that has been computed */

/* expand the caches to at least n */
static void ensure_cache(idx_t n)
{
    /* initial allocation */
    if (cap == 0) {
        size_t init = 16;
        if ((size_t)n + 1 > init)
            init = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, init * sizeof(seq_t));
        w_cache = (seq_t *)realloc(w_cache, init * sizeof(seq_t));
        if (!a_cache || !w_cache)
            abort();
        cap   = init;
        a_cache[0] = (seq_t)1;       /* a(0) = 1 */
        w_cache[0] = (seq_t)0;       /* unused, for safety */
        max_n = 0;
    }
    /* grow if necessary */
    else if (n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        w_cache = (seq_t *)realloc(w_cache, new_cap * sizeof(seq_t));
        if (!a_cache || !w_cache)
            abort();
        cap = new_cap;
    }
    /* nothing to do if already computed */
    if (n <= max_n)
        return;
    /* compute A007526 values for new indices */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t t = w_cache[k - 1];
        t *= (seq_t)k;
        t += (seq_t)k;
        w_cache[k] = t;
    }
    /* compute the sequence values for new indices */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* binomial(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = w_cache[k];
            term *= binom;
            term *= a_cache[m - k];
            sum += term;
            if (k < m) {
                seq_t num = binom * (seq_t)(m - k);
                binom     = num / (seq_t)k;   /* exact division */
            }
        }
        a_cache[m] = sum;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
