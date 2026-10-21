/*
 * A267159  Total number of ON (black) cells after n iterations of the "Rule
 *          107" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) (n > 12)
 *            Initial values: a(0)=1, a(1)=2, a(2)=5, a(3)=10, a(4)=16,
 *            a(5)=24, a(6)=31, a(7)=44, a(8)=50, a(9)=65, a(10)=73, a(11)=94,
 *            a(12)=100 The sequence is computed on demand and memoised
 *            between calls
 * Range:     u8 n <= 20, u16 n <= 358, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* initialise cache with first 13 terms (0..12) */
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory → return 0 */

        cache[0] = 1;  cache[1] = 2;  cache[2] = 5;
        cache[3] = 10; cache[4] = 16; cache[5] = 24;
        cache[6] = 31; cache[7] = 44; cache[8] = 50;
        cache[9] = 65; cache[10] = 73; cache[11] = 94;
        cache[12] = 100;
    }

    /* Return value if already computed */
    if ((size_t)n < cache_cap)
        return cache[n];

    /* grow the cache to hold up to index n */
    size_t new_cap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) return (seq_t)0;   /* out of memory → return 0 */
    cache_cap = new_cap;

    /* compute missing terms using the recurrence */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4]
                 - cache[i-5] - cache[i-6] + cache[i-7];
    }

    return cache[n];
}
