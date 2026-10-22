/*
 * A313172  Coordination sequence Gal.5.234.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-2) + 2*a(n-3) - a(n-4) + 4*a(n-5) - a(n-6) +
 *            2*a(n-7) - 2*a(n-8) - a(n-10) Initial terms: a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=15, a(4)=18, a(5)=22, a(6)=32, a(7)=35, a(8)=35,
 *            a(9)=45, a(10)=51, a(11)=52, a(12)=58, a(13)=62, a(14)=69
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init_values[15] = {
    1ULL, 4ULL, 9ULL, 15ULL, 18ULL, 22ULL,
    32ULL, 35ULL, 35ULL, 45ULL, 51ULL, 52ULL,
    58ULL, 62ULL, 69ULL
};

seq_t a(idx_t n)
{
    /* Return a known initial term directly */
    if (n < 15)
        return init_values[(size_t)n];

    /* Dynamic cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed_max = 0;   /* highest index already computed */

    /* Allocate and initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init_values[i];
        computed_max = 14;   /* indices 0..14 are ready */
    }

    /* Grow the cache if needed */
    size_t N = (size_t)n;
    if (N >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= N)
            new_cap = new_cap ? new_cap * 2 : 15;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested term was already computed, simply return it */
    if (N <= computed_max)
        return cache[N];

    /* Compute all terms up to N using the recurrence */
    for (size_t i = computed_max + 1; i <= N; ++i) {
        seq_t pos = (seq_t)2 * cache[i - 3]
                  + (seq_t)4 * cache[i - 5]
                  + (seq_t)2 * cache[i - 7];
        seq_t neg = (seq_t)2 * cache[i - 2]
                  + cache[i - 4]
                  + cache[i - 6]
                  + (seq_t)2 * cache[i - 8]
                  + cache[i - 10];
        cache[i] = pos - neg;
    }
    computed_max = N;
    return cache[N];
}
