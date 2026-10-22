/*
 * A354394  Expansion of e.g.f. 1/(1 + (exp(x) - 1)^5 / 120).
 * https://oeis.org/A354394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -Sum_{k=1..n} binomial(n,k) * Stirling2(k,5) *
 *            a(n-k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence and the Stirling numbers S(n,5)
 * (and the intermediate S(n,0..5)) in dynamic arrays that grow as needed.
 * It is fully valid in C11 and C++17 when seq_t is any integer type or
 * arbitrary‑precision class.  The only required headers are <stdlib.h>
 * for malloc/realloc and <stddef.h> for size_t, which is already available
 * in both language modes.  No external utilities are used. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static cache for the sequence a(n) ---------- */
    static seq_t *cache      = NULL;   /* array of a(0..cache_len-1) */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static size_t cache_len  = 0;      /* number of values already computed */

    /* ---------- static cache for Stirling numbers S(i,k), 0 <= k <= 5 ---------- */
    static seq_t *stir_cache  = NULL;  /* linearised array with stride 6 */
    static size_t stir_cap    = 0;      /* capacity in seq_t units */
    static idx_t  stir_max    = 0;      /* largest i fully computed */

    /* ------ helper to ensure cache capacity ------ */
    if (cache_cap <= n) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */
        /* zero the newly allocated part */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* ------ helper to ensure Stirling cache capacity ------ */
    if (stir_cap / 6 <= n) {
        size_t rows_needed = n + 1;                      /* indices 0..n */
        size_t new_cap = rows_needed * 6;                /* each row has 6 entries */
        while (stir_cap < new_cap) stir_cap = (stir_cap == 0) ? 4 : stir_cap * 2;
        stir_cache = (seq_t*)realloc(stir_cache, stir_cap * sizeof(seq_t));
        if (!stir_cache) abort();
        /* zero the newly allocated part */
        for (size_t i = (stir_cap / 2) * 6; i < new_cap; ++i) stir_cache[i] = 0;
    }

    /* ------ compute Stirling numbers up to n if needed ------ */
    if (stir_max < n) {
        /* ensure row 0 is initialised */
        if (stir_max == 0) {
            stir_cache[0*6 + 0] = 1;  /* S(0,0)=1 */
            for (int k = 1; k <= 5; ++k) stir_cache[0*6 + k] = 0;
        }
        for (idx_t i = stir_max + 1; i <= n; ++i) {
            stir_cache[i*6 + 0] = 0;          /* S(i,0)=0 for i>0 */
            for (int k = 1; k <= 5; ++k) {
                stir_cache[i*6 + k] =
                    stir_cache[(i-1)*6 + k] * (seq_t)k   /* k * S(i-1,k) */
                    + stir_cache[(i-1)*6 + (k-1)];       /* + S(i-1,k-1) */
            }
        }
        stir_max = n;
    }

    /* ------ compute the sequence values up to n if needed ------ */
    if (cache_len <= n) {
        /* initialise a(0) if not already set */
        if (cache_len == 0) {
            cache[0] = 1;
            cache_len = 1;
        }
        /* compute subsequent terms */
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            seq_t sum = 0;
            seq_t c   = 1;          /* binomial coefficient C(cur,0) */

            for (idx_t k = 1; k <= cur; ++k) {
                /* update c to C(cur,k) using C(cur,k)=C(cur,k-1)*(cur-k+1)/k */
                c = c * (seq_t)(cur - k + 1) / (seq_t)k;

                seq_t s5 = stir_cache[k*6 + 5];
                if (s5 == 0) continue;          /* skip zero Stirling numbers */

                seq_t term = c * s5 * cache[cur - k];
                sum += term;
            }
            cache[cur] = -sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
