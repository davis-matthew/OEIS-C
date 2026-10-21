/*
 * A264122  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 1,-2 or 2,-1.
 * https://oeis.org/A264122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) + 16*a(n-4) + 4*a(n-5) +
 *            a(n-6) - 4*a(n-7) - a(n-8) for n > 8 with base values: a(1)=2,
 *            a(2)=9, a(3)=45, a(4)=200, a(5)=845, a(6)=3549, a(7)=14994,
 *            a(8)=63525
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based array of computed terms */
static size_t cache_cap = 0;      /* allocated size (index 0 not used) */
static size_t cache_known = 0;    /* number of terms computed (last index) */

/* Ensure the cache has room for up to 'needed' indices (1‑based). */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) {
        /* Allocation failed; in a real program you might abort or handle error. */
        abort();
    }
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* out of range, not defined by OEIS */

    size_t idx = (size_t)n;

    /* Base values for the first 8 terms */
    static const seq_t base[9] = {0, 2, 9, 45, 200, 845, 3549, 14994, 63525};

    if (idx <= 8)                   /* direct lookup */
        return base[idx];

    /* If we already know the value, return it immediately */
    if (idx <= cache_known)
        return cache[idx];

    /* Ensure the cache can hold up to 'idx' */
    ensure_cache(idx);

    /* Compute missing terms up to 'idx' */
    for (size_t i = cache_known + 1; i <= idx; ++i) {
        if (i <= 8) {
            cache[i] = base[i];
            continue;
        }
        seq_t t = 4 * cache[i - 1];
        t += cache[i - 2];
        t -= 4 * cache[i - 3];
        t += 16 * cache[i - 4];
        t += 4 * cache[i - 5];
        t += cache[i - 6];
        t -= 4 * cache[i - 7];
        t -= cache[i - 8];
        cache[i] = t;
    }
    cache_known = idx;
    return cache[idx];
}
