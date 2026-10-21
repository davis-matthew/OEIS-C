/*
 * A280856  Number of nX5 0..2 arrays with no element equal to more than one
 *          of its king-move neighbors and with new values introduced in order
 *          0 sequentially upwards.
 * https://oeis.org/A280856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 35, u64 n <= 77, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A280856 – Number of n×5 arrays with elements 0..2,
 *  no element equal to more than one of its king‑move neighbors,
 *  with new values introduced in order 0 sequentially upwards.
 *
 *  Empirical recurrence (valid for n > 18):
 *     a(n) = a(n‑1) + a(n‑2) – a(n‑3) + 2·a(n‑4) – a(n‑5)
 *             + 4·a(n‑6) – a(n‑7) – a(n‑8) – 2·a(n‑9) – 3·a(n‑12)
 *  Initial values (n = 1 … 18):
 *     30, 442, 256, 216, 282, 491, 968, 1857, 3220, 5281,
 *     8574, 14339, 24694, 42833, 73650, 125361, 212438, 360727
 */

#include <stdlib.h>

/* The types idx_t and seq_t are provided by the build system. */

/* First 18 known terms – these seed the recurrence.  They are
 * constants of the underlying seq_t type; no computation is needed. */
static const seq_t init[18] = {
    30ULL, 442ULL, 256ULL, 216ULL, 282ULL, 491ULL, 968ULL, 1857ULL,
    3220ULL, 5281ULL, 8574ULL, 14339ULL, 24694ULL, 42833ULL, 73650ULL,
    125361ULL, 212438ULL, 360727ULL
};

/* Dynamic cache to memoise computed values.  The array grows as required. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of valid entries in cache (1‑based) */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Resize the cache array so that it can hold at least `needed` elements. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp)
        abort();                /* out of memory */
    *ptr = tmp;
    *cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();                /* undefined for n < 1 */

    /* Return known initial values directly. */
    if (n <= 18)
        return init[(size_t)n - 1];

    /* Initialise cache with the 18 known values on first use. */
    if (cache_len == 0) {
        ensure_capacity(18, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i)
            cache[i] = init[i];
        cache_len = 18;
    }

    /* Extend cache up to the requested index if necessary. */
    if ((size_t)n > cache_len) {
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t j = cache_len; j < (size_t)n; ++j) {
            /* indicesj-? are always valid because j >= 18 */
            seq_t val = cache[j - 1]          /* a(n‑1) */
                      + cache[j - 2]          /* a(n‑2) */
                      - cache[j - 3]          /* a(n‑3) */
                      + (seq_t)2 * cache[j - 4]   /* 2·a(n‑4) */
                      - cache[j - 5]          /* a(n‑5) */
                      + (seq_t)4 * cache[j - 6]   /* 4·a(n‑6) */
                      - cache[j - 7]          /* a(n‑7) */
                      - cache[j - 8]          /* a(n‑8) */
                      - (seq_t)2 * cache[j - 9]   /* 2·a(n‑9) */
                      - (seq_t)3 * cache[j - 12]; /* 3·a(n‑12) */
            cache[j] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
