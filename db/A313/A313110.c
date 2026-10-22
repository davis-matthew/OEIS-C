/*
 * A313110  Coordination sequence Gal.3.13.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence:  A313110  -  Coordination sequence Gal.3.13.1
    Formula:  a(0) = 1,  a(1) = 4
              For n ≥ 2:
                 if ((n-2) mod 6) < 4 then a(n) = a(n-1) + 5
                 else                     a(n) = a(n-1) + 4
*/
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_used = 0;

/* Ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Resize if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 64;
        while (new_size <= (size_t)n + 1)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return;                /* allocation failed – caller will fail gracefully */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first two entries if not already done */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        cache_used = 1;
    }
    if (cache_used == 1) {
        cache[1] = (seq_t)4;
        cache_used = 2;
    }

    /* Compute missing values */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        if (i >= 2) {
            size_t mod = (i - 2) % 6;
            if (mod < 4)
                cache[i] = cache[i - 1] + (seq_t)5;
            else
                cache[i] = cache[i - 1] + (seq_t)4;
        }
    }
    cache_used = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
