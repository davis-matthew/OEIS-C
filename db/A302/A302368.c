/*
 * A302368  Number of 2Xn 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)-4*a(n-3)-4*a(n-4) a(1) = 2, a(2) = 2,
 *            a(3) = 12, a(4) = 20
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* maximum index stored + 1 (0..cap-1) */

/* ensure cache can hold values up to idx (0‑based) */
static void ensure_cache(size_t idx)
{
    if (idx + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        cache_cap = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* fatal: cannot allocate */
        cache = new_cache;
        /* initialise new entries to 0 */
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = 0;
    }
}

/* memoised recurrence for A302368 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe fallback */

    /* base values */
    if (n == 1) return 2;
    if (n == 2) return 2;
    if (n == 3) return 12;
    if (n == 4) return 20;

    /* use 0‑based indexing: cache[i] holds a(i) */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    if (cache[idx] != 0) return cache[idx];

    /* compute using 1‑based indices but store in 0‑based array */
    seq_t a1 = cache[1];   /* a(1) */
    seq_t a2 = cache[2];   /* a(2) */
    seq_t a3 = cache[3];   /* a(3) */
    seq_t a4 = cache[4];   /* a(4) */
    /* initialise cache for base cases if not already stored */
    if (a1 == 0) a1 = 2, cache[1] = 2;
    if (a2 == 0) a2 = 2, cache[2] = 2;
    if (a3 == 0) a3 = 12, cache[3] = 12;
    if (a4 == 0) a4 = 20, cache[4] = 20;

    for (size_t k = 5; k <= idx; ++k) {
        seq_t v = 2 * cache[k - 1] + 4 * cache[k - 2]
                - 4 * cache[k - 3] - 4 * cache[k - 4];
        cache[k] = v;
    }
    return cache[idx];
}
