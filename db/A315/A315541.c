/*
 * A315541  Coordination sequence Gal.4.136.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8
 * Initial values:
 *   a(0) = 1, a(1) = 6, a(2) = 11, a(3) = 17, a(4) = 23,
 *   a(5) = 29, a(6) = 34, a(7) = 40, a(8) = 46
 */

#include <stdlib.h>

/* static cache to memoise previously computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of allocated slots */
static size_t cache_size = 0;  /* number of computed terms */

/* pre‑computed initial terms */
static const seq_t initial[9] = {1U, 6U, 11U, 17U, 23U, 29U, 34U, 40U, 46U};

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – abort the program */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* lazily grow the cache to include n */
    if (n >= cache_size) {
        ensure_cache_cap((size_t)n + 1);
        /* fill missing terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i < 9) {
                cache[i] = initial[i];
            } else {
                /* recurrence: a(n) = a(n-1) + a(n-7) - a(n-8) */
                seq_t val = cache[i - 1] + cache[i - 7] - cache[i - 8];
                cache[i] = val;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
