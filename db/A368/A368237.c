/*
 * A368237  Expansion of e.g.f. 1/(exp(-x) - 3*x).
 * https://oeis.org/A368237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*a(n-1) + Σ_{k=1..n} (-1)^(k-1) * binomial(n,k) *
 *            a(n-k) Initial value: a(0) = 1 (OEIS A368237 – Expansion of
 *            e.g.f. 1/(exp(-x) – 3*x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Static storage for memoised values.  */
static seq_t       *seq_cache = NULL;   /* array of computed a(i)          */
static size_t        cache_len    = 0;   /* next free index (i.e. #computed)*/
static size_t        cache_cap    = 0;   /* allocated capacity of seq_cache*/

static void ensure_cache_cap(idx_t n)
{
    /* need capacity for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    /* double size until large enough (or use needed if >2) */
    size_t new_cap = needed > 2 ? needed * 2 : 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/*  Compute a(n) using the recurrence and memoisation.  */
seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (cache_len == 0) {
        ensure_cache_cap(0);
        seq_cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_len   = 1;
    }

    /* grow cache if needed */
    ensure_cache_cap(n);

    /* compute any missing values from cache_len up to n (inclusive) */
    for (idx_t m = cache_len; m <= n; ++m) {
        seq_t sum = 0;                 /* Σ contribution with alternating sign */

        seq_t binom = 1;                /* binomial(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            /* binom = C(m,k) = C(m,k-1) * (m-k+1) / k  */
            binom = binom * (seq_t)(m - k + 1);
            binom = binom / (seq_t)k;

            seq_t term = binom * seq_cache[m - k];
            if (k & 1)                 /* odd k → positive sign */
                sum += term;
            else                       /* even k → negative sign */
                sum -= term;
        }

        /* recurrence: a(m) = 3*m*a(m-1) + sum */
        seq_t val = ((seq_t)3 * (seq_t)m * seq_cache[m - 1]) + sum;
        seq_cache[m] = val;
    }

    cache_len = (size_t)n + 1;          /* all indices up to n are now filled */
    return seq_cache[n];
}
