/*
 * A393021  List points (x,y,z) having integer coordinates, sorted first by L
 *          = |x| + |y| + |z| and in case of ties, then by z and last by polar
 *          angle 0 <= phi < 2*Pi in a polar coordinate system. Sequence gives
 *          x-coordinates.
 * https://oeis.org/A393021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9746, i16 n <= 9746, i32 n <= 9746, i64 n <= 9746,
 *            i128 n <= 9746, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A393021: the x‑coordinate of the integer
   lattice point (x,y,z) sorted by
   1. L = |x|+|y|+|z|
   2. z (increasing)
   3. polar angle φ measured from the positive y‑axis clockwise
   (i.e., φ=0 for (0, >0), φ=π/2 for (>0, 0), φ=π for (0, <0), φ=3π/2
   for (<0, 0)).  This implementation generates each layer L
   deterministically, so no floating‑point or sorting is needed. */

#include <stdlib.h>

/* Static cache for computed x‑coordinates. */
static seq_t  *cache   = NULL;   /* array of x‑coordinates */
static size_t cache_len = 0;     /* number of valid elements */
static size_t cache_cap = 0;     /* allocated capacity */

static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed: reset cache */
        free(cache);
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

static void cache_append(seq_t value)
{
    ensure_cache_size(cache_len + 1);
    if (cache) cache[cache_len++] = value;
}

/* Generate all points with Manhattan distance equal to L, in the required
   local order (by z, then by φ) and append their x‑coordinates to the cache. */
static void generate_layer(idx_t L)
{
    for (int z = -(int)L; z <= (int)L; ++z) {
        int r;                         /* |x|+|y| for this z */
        if (z >= 0)
            r = (int)(L - (idx_t)z);
        else
            r = (int)(L + (idx_t)z);

        if (r == 0) {                  /* single point (0,0) */
            cache_append((seq_t)0);
            continue;
        }

        /* 1. from top to right: (x, r‑x) for x=0..r */
        for (int x = 0; x <= r; ++x) {
            cache_append((seq_t)x);
        }
        /* 2. right to bottom: (x, -(r‑x)) for x=r‑1..0 */
        for (int x = r - 1; x >= 0; --x) {
            cache_append((seq_t)x);
        }
        /* 3. bottom to left: (x, -(r + x)) for x=-1..-r */
        for (int x = -1; x >= -r; --x) {
            cache_append((seq_t)x);
        }
        /* 4. left to top: (x, r + x) for x=-r+1..-1 */
        for (int x = -r + 1; x < 0; ++x) {
            cache_append((seq_t)x);
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Generate terms up to index n if not already cached. */
    static idx_t next_L = 0;   /* next layer to generate */
    while (cache_len <= n) {
        generate_layer(next_L);
        ++next_L;
    }
    return cache[n];
}
