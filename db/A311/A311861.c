/*
 * A311861  Coordination sequence Gal.6.130.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4)-a(n-6)+a(n-7)+a(n-9)-a(n-10) for n >
 *            10; initial values: a(0)=1,a(1)=4,a(2)=8,a(3)=13,a(4)=17,
 *            a(5)=21,a(6)=26,a(7)=31,a(8)=35,a(9)=39,a(10)=44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed values */
static size_t  cache_cap  = 0;    /* allocated capacity */
static idx_t   cache_len  = 0;    /* number of values stored */

/* Ensure that seq_cache has at least 'needed' slots. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Coordination sequence for Gal.6.130.2 */
seq_t a(idx_t n)
{
    /* Return from cache if already computed. */
    if (n < cache_len) return seq_cache[(size_t)n];

    /* Initial values known a priori. */
    static const seq_t init_vals[11] = {
        (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)13, (seq_t)17,
        (seq_t)21, (seq_t)26, (seq_t)31, (seq_t)35, (seq_t)39, (seq_t)44
    };

    /* Extend cache to accommodate up to index n. */
    size_t needed = (size_t)n + 1;
    ensure_cache_capacity(needed);

    /* Compute missing entries. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 10) {
            seq_cache[i] = init_vals[i];
        } else {
            seq_t pos = seq_cache[i-1] + seq_cache[i-3]
                      + seq_cache[i-7] + seq_cache[i-9];
            seq_t neg = seq_cache[i-4] + seq_cache[i-6]
                      + seq_cache[i-10];
            seq_cache[i] = pos - neg;
        }
    }

    cache_len = (idx_t)needed;
    return seq_cache[(size_t)n];
}
