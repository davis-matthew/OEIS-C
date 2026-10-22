/*
 * A310512  Coordination sequence Gal.6.352.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10); Initial
 *            values: a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=21, a(5)=26,
 *            a(6)=31, a(7)=36, a(8)=42, a(9)=48, a(10)=52. Reference: OEIS
 *            A310512
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)16, (seq_t)21,
    (seq_t)26, (seq_t)31, (seq_t)36, (seq_t)42, (seq_t)48, (seq_t)52
};

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache      = NULL;   /* buffer holding terms */
    static idx_t cache_len   = 0;      /* number of slots currently allocated */
    static idx_t computed_max/* highest index that has been computed*/ = (idx_t)-1;
    static bool  init_done   = false;

    /* Initialise the first 11 terms once */
    if (!init_done) {
        size_t init_size = 11;          /* indices 0..10 inclusive */
        cache = (seq_t *)realloc(cache, init_size * sizeof(seq_t));
        if (!cache) exit(1);            /* out‑of‑memory */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        cache_len   = init_size;
        computed_max = 10;
        init_done = true;
    }

    /* Ensure the array is large enough for n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        /* Zero initialise the newly allocated part */
        if (new_len > cache_len)
            memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* If the term is already known, return it */
    if (n <= computed_max)
        return cache[n];

    /* Compute terms from computed_max+1 up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] =  2 * cache[i-1]
                   - 2 * cache[i-2]
                   + 2 * cache[i-3]
                   - 2 * cache[i-4]
                   + 2 * cache[i-5]
                   - 2 * cache[i-6]
                   + 2 * cache[i-7]
                   - 2 * cache[i-8]
                   + 2 * cache[i-9]
                   -   cache[i-10];
    }

    computed_max = n;
    return cache[n];
}
