/*
 * A312909  Coordination sequence Gal.6.150.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312909
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

/* Recurrence: a(n) = a(n-1)+a(n-3)-a(n-4)-a(n-6)+a(n-7)+a(n-9)-a(n-10) for n>10
   Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=17, a(5)=23,
   a(6)=27, a(7)=31, a(8)=37, a(9)=41, a(10)=45 */

#include <stdlib.h>
#include <stddef.h>

/* Memoization cache */
static seq_t *cache     = NULL;   /* array of cached terms */
static size_t cache_len = 0;      /* number of elements currently computed */
static size_t cache_cap = 0;      /* allocated array capacity */

/* Grow the cache array to hold at least 'needed' entries */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL && new_cap > 0)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term of A312909 */
seq_t a(idx_t n)
{
    /* Initial terms (indices 0..10) */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)4,  (seq_t)9,  (seq_t)13, (seq_t)17,
        (seq_t)23, (seq_t)27, (seq_t)31, (seq_t)37, (seq_t)41,
        (seq_t)45
    };

    size_t needed = (size_t)(n + 1);            /* indices 0 .. n inclusive */

    if (needed > cache_len) {
        ensure_cache(needed);

        /* Fill missing values up to index n */
        for (size_t i = cache_len; i < needed; ++i) {
            if (i < 11) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-1] + cache[i-3]
                          - cache[i-4] - cache[i-6]
                          + cache[i-7] + cache[i-9]
                          - cache[i-10];
            }
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
