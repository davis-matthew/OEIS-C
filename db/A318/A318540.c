/*
 * A318540  Number of nX3 0..1 arrays with every element unequal to 0, 1, 4 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) + 3*a(n-3) + a(n-4) + 3*a(n-5) + a(n-7) for n >
 *            8 * Initial values (n ≥ 1): * a(1)=3, a(2)=3, a(3)=6, a(4)=13,
 *            a(5)=26, a(6)=52, a(7)=107, a(8)=218
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

/* ---- static cache --------------------------------------------- */
static seq_t *cache = NULL;          /* array of a(n) values (index 0 unused) */
static idx_t  cache_len = 0;         /* current number of valid entries */
static idx_t  cache_cap = 0;         /* current allocated capacity */

/* ---- ensure values for all indices 0 … n are available  -------- */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if we already have enough entries. */
    if (cache_len > n) return;

    /* Grow the allocation if necessary. */
    if (cache_cap < (idx_t)(n + 1)) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Guarantee that index 0 is present (unused but simplifies indexing). */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Pre‑defined initial values for the first eight terms. */
    static const seq_t init[9] = {
        0,    /* unused placeholder for index 0 */
        3,    /* a(1) */
        3,    /* a(2) */
        6,    /* a(3) */
        13,   /* a(4) */
        26,   /* a(5) */
        52,   /* a(6) */
        107,  /* a(7) */
        218   /* a(8) */
    };

    /* Compute new terms starting at the current cache_len. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= (idx_t)8) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1]
                     + (seq_t)3 * cache[i-3]
                     +       cache[i-4]
                     + (seq_t)3 * cache[i-5]
                     +       cache[i-7];
        }
    }

    cache_len = n + 1;      /* now cache contains indices 0 … n */
}

/* ---- public sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* behaviour below the offset is irrelevant */
    ensure_cache(n);
    return cache[n];
}
