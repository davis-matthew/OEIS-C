/*
 * A313824  Coordination sequence Gal.6.195.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10), a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=20, a(5)=24,
 *            a(6)=29, a(7)=34, a(8)=38, a(9)=42, a(10)=48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 10 */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)16, (seq_t)20,
        (seq_t)24, (seq_t)29, (seq_t)34, (seq_t)38, (seq_t)42,
        (seq_t)48
    };

    /* Cache for computed terms beyond the initial block */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache */
    static idx_t max_cached = 0;    /* highest index cached (>=10 after init) */

    /* Return an initial term directly for n <= 10 */
    if (n <= 10)
        return init[(size_t)n];

    /* Ensure the cache is allocated and initialised */
    if (max_cached < 10) {
        /* Allocate at least 16 entries for convenience */
        size_t new_cap = 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;   /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cap = new_cap;

        /* Copy the initial 11 values into the cache */
        for (size_t i = 0; i <= 10; ++i)
            cache[i] = init[i];

        max_cached = 10;
    }

    /* Grow the cache if we need more space */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3]
                 - cache[i - 4] - cache[i - 6]
                 + cache[i - 7] + cache[i - 9]
                 - cache[i - 10];
    }
    max_cached = n;

    return cache[n];
}
