/*
 * A311057  Coordination sequence Gal.4.18.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311057
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

/* Recurrence:
   a(n) = a(n-1) – a(n-2) + a(n-3) – a(n-4) + 2*a(n-5) – a(n-6) +
          a(n-7) – a(n-8) + a(n-9) – a(n-10)  for n > 14
   Initial values:
   a(0)=1, a(1)=4, a(2)=8, a(3)=11, a(4)=15, a(5)=21,
   a(6)=26, a(7)=30, a(8)=34, a(9)=38, a(10)=43,
   a(11)=48, a(12)=51, a(13)=54, a(14)=59.
*/
#include <stdlib.h>

static const seq_t init_vals[15] = {
    1, 4, 8, 11, 15, 21, 26, 30, 34, 38,
    43, 48, 51, 54, 59
};

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_size = 0; /* number of values stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* ensure that the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed > cache_cap
                     ? needed
                     : (cache_cap ? cache_cap * 2 : needed);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* return the n-th term of sequence A311057 */
seq_t a(idx_t n)
{
    /* first 15 terms are known explicitly */
    if (n < 15u)
        return init_vals[(size_t)n];

    /* initialise cache on first use */
    if (!cache) {
        ensure_cache(n);
        /* copy the initial block into cached memory */
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init_vals[i];
        cache_size = 15;
    } else {
        ensure_cache(n);
    }

    /* compute missing terms up to n */
    size_t up_to = (size_t)n;
    for (size_t i = cache_size; i <= up_to; ++i) {
        cache[i] = cache[i - 1]
                 - cache[i - 2]
                 + cache[i - 3]
                 - cache[i - 4]
                 + 2 * cache[i - 5]
                 - cache[i - 6]
                 + cache[i - 7]
                 - cache[i - 8]
                 + cache[i - 9]
                 - cache[i -10];
    }
    cache_size = up_to + 1;
    return cache[up_to];
}
