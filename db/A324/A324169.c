/*
 * A324169  Number of labeled graphs covering the vertex set {1,...,n} with no
 *          crossing edges.
 * https://oeis.org/A324169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 1. For n ≥ 3: a(n) = 2*a(n‑1) +
 *            a(n‑2) + Σ_{i=3..n} (a(i‑1)+a(i‑2)) *
 *            (a(n‑i+2)+2*a(n‑i+1)+a(n‑i))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Dynamically expand the cache so that indices 0..n can be stored. */
static void ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if (*cache == NULL) {
        *cap = 0;
    }
    if (*cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* insufficient memory */
        *cache = tmp;
        *cap   = new_cap;
    }
}

/* Compute a(n) for sequence A324169. */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t last       = (idx_t)-1;   /* highest index computed so far */

    /* Make room for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base terms if not done yet */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)0;
        if (n >= 2) cache[2] = (seq_t)1;
        last = n >= 2 ? 2 : (n >= 1 ? 1 : 0);
    }

    /* Compute missing terms up to n */
    if (last < n) {
        for (idx_t j = last + 1; j <= n; ++j) {
            if (j == 0) {
                cache[0] = (seq_t)1;
            } else if (j == 1) {
                cache[1] = (seq_t)0;
            } else if (j == 2) {
                cache[2] = (seq_t)1;
            } else {
                seq_t sum_s = (seq_t)0;
                for (idx_t i = 3; i <= j; ++i) {
                    seq_t term1 = cache[i - 1] + cache[i - 2];
                    seq_t term2 = cache[j - i + 2] + (seq_t)2 * cache[j - i + 1] + cache[j - i];
                    sum_s += term1 * term2;
                }
                cache[j] = (seq_t)2 * cache[j - 1] + cache[j - 2] + sum_s;
            }
        }
        last = n;
    }

    return cache[n];
}
