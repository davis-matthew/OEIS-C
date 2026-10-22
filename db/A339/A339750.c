/*
 * A339750  Number of (undirected) paths in the 2 X n king graph.
 * https://oeis.org/A339750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 a(n‑1) - 33 a(n‑2) + 40 a(n‑3) - 32 a(n‑5) + 16 a(n‑6)
 *            (n ≥ 7) with initial terms a(1)=1, a(2)=30, a(3)=235, a(4)=1448,
 *            a(5)=7909, a(6)=40674 (Indices are 1‑based; a(0) is unused.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values, 1‑based indexing (cache[0] unused) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size (size_t), i.e. number of elements */
static size_t cache_len = 0;   /* highest index that has a value */

/* the first six terms, index 0 unused */
static const seq_t init_vals[7] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)30,
    (seq_t)235,
    (seq_t)1448,
    (seq_t)7909,
    (seq_t)40674
};

/* Ensure that cache has been allocated up to include index n,
   growing it if necessary and computing any new terms. */
static void ensure_capacity(idx_t n)
{
    if (n < 1) return;

    size_t needed = (size_t)n + 1;       /* include index 0 */
    if (cache == NULL) {
        /* first allocation – at least large enough for n */
        cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (cache == NULL) return;       /* allocation failed – caller will see 0 */

        /* initialise first six terms (if they exist) */
        size_t i;
        for (i = 1; i <= 6 && i < needed; ++i) cache[i] = init_vals[i];
        cache_len = (i <= 6) ? (size_t)i - 1 : 6;
        cache_cap = needed;

    } else if (needed > cache_cap) {
        /* enlarge existing array */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return;           /* reallocation failed – give up */
        cache = tmp;
        cache_cap = needed;

        /* initialise new slots if we were previously empty */
        if (cache_len < 6) {
            size_t i;
            for (i = cache_len + 1; i <= 6 && i < needed; ++i)
                cache[i] = init_vals[i];
            cache_len = (i <= 6) ? (size_t)i - 1 : 6;
        }
    }

    /* compute any missing terms beyond cache_len up to n */
    if ((idx_t)cache_len < n) {
        idx_t start = (cache_len + 1 > (idx_t)6) ? cache_len + 1 : (idx_t)7;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 10 * cache[i-1];
            val -= 33 * cache[i-2];
            val += 40 * cache[i-3];
            val -= 32 * cache[i-5];
            val += 16 * cache[i-6];
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_capacity(n);
    return cache[(size_t)n];
}
