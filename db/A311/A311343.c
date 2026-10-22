/*
 * A311343  Coordination sequence Gal.4.60.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) + a(n-7) -
 *            a(n-8) + a(n-9) - a(n-10) + a(n-11) - a(n-12) (for n > 12).
 *            Initial values: a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=16,
 *            a(5)=22, a(6)=26, a(7)=30, a(8)=34, a(9)=36, a(10)=40, a(11)=46,
 *            a(12)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* pointer to dynamically allocated array */
static size_t cache_capacity = 0;/* allocated size of cache */
static size_t cache_last = 0;     /* last computed index (0‑based) */

/* Allocate and initialise cache with the first 13 terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* initial capacity sufficient for indices up to 12 */
    cache_capacity = 64;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (!cache)
        abort();

    /* fill in the initial terms */
    cache[0]  = (seq_t)1;
    cache[1]  = (seq_t)4;
    cache[2]  = (seq_t)8;
    cache[3]  = (seq_t)12;
    cache[4]  = (seq_t)16;
    cache[5]  = (seq_t)22;
    cache[6]  = (seq_t)26;
    cache[7]  = (seq_t)30;
    cache[8]  = (seq_t)34;
    cache[9]  = (seq_t)36;
    cache[10] = (seq_t)40;
    cache[11] = (seq_t)46;
    cache[12] = (seq_t)52;

    cache_last = 12; /* indices 0..12 have been computed */
}

/* Ensure that the cache can hold at least indices up to 'max_index'. */
static void ensure_capacity(size_t max_index)
{
    if (max_index < cache_capacity)
        return;
    size_t new_cap = cache_capacity;
    while (new_cap <= max_index)
        new_cap <<= 1;        /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute the sequence value at index n. */
seq_t a(idx_t n)
{
    init_cache();

    /* for small n, the value is already present */
    if (n <= 12)
        return cache[(size_t)n];

    size_t target = (size_t)n;
    if (target > cache_last) {
        ensure_capacity(target);
        for (size_t i = cache_last + 1; i <= target; ++i) {
            cache[i] = cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4]
                       + cache[i-5] + cache[i-7] - cache[i-8]
                       + cache[i-9] - cache[i-10] + cache[i-11]
                       - cache[i-12];
        }
        cache_last = target;
    }
    return cache[target];
}
