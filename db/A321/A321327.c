/*
 * A321327  Expansion of Product_{k>=0} (1 - x^(2^k))^(2^k).
 * https://oeis.org/A321327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n > 0, let b(n) = { Σ_{i=0}^{n/2} a(i)·a(n/2-i) if
 *            n even 0 if n odd } Then a(n) = b(n) – b(n‑1)
 * Range:     i8 n <= 25, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;    /* size of allocated cache in terms   */
static idx_t  computed  = 0;      /* number of terms computed so far    */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache && (size_t)n < cache_alloc)
        return;
    size_t new_cap = cache_alloc ? cache_alloc : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;              /* allocation failure; keep 0 cache */
    if (cache) {
        /* copy old data */
        for (idx_t i = 0; i < computed; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_alloc = new_cap;
}

/* Return the n‑th term of OEIS A321327. */
seq_t
a(idx_t n)
{
    ensure_cache(n);

    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            __int128 sum_n   = 0;  /* b(n)   */
            __int128 sum_nm1 = 0;  /* b(n-1) */

            /* compute b(n)   */
            if ((i & 1) == 0) { /* even */
                idx_t k = i >> 1;
                for (idx_t j = 0; j <= k; ++j)
                    sum_n += (__int128)cache[j] * (__int128)cache[k - j];
            }

            /* compute b(n-1) */
            if (((i - 1) & 1) == 0) { /* even */
                idx_t k = (i - 1) >> 1;
                for (idx_t j = 0; j <= k; ++j)
                    sum_nm1 += (__int128)cache[j] * (__int128)cache[k - j];
            }

            seq_t val = (seq_t)(sum_n - sum_nm1);
            cache[i] = val;
        }
    }
    computed = n + 1;
    return cache[n];
}
