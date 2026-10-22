/*
 * A333833  Number of permutations p of [n] such that |p(i) - p(i-1)| <= 2 and
 *          |p(i) - p(i-2)| <= 3.
 * https://oeis.org/A333833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 65, u64 n <= 134,
 *            u128 n <= 271, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A333833
   Number of permutations p of [n] such that |p(i)-p(i-1)|<=2 and |p(i)-p(i-2)|<=3.
   Recurrence (valid for n>=9):  a(n) = a(n-1) + a(n-4)
   Base values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=12,
                a(5)=14, a(6)=18, a(7)=28, a(8)=42
   Offset: 0
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..8 */
    static const idx_t base_vals[9] = {1,1,2,6,12,14,18,28,42};

    /* Fast return for small n, no caching needed */
    if (n < 9)
        return (seq_t)base_vals[n];

    /* Static cache for recurrence */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0; /* number of filled entries */
    static idx_t cache_cap  = 0; /* allocated capacity */

    /* Initial allocation of base values */
    if (!cache) {
        cache_cap  = 9;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                    /* allocation failure */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = (seq_t)base_vals[i];
        cache_size = 9;
    }

    /* Ensure capacity for all entries up to n */
    if (cache_cap < n + 1) {
        /* Double capacity until sufficient (never below 9) */
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        if (new_cap < 9)
            new_cap = 9;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend cache using recurrence a(n) = a(n-1) + a(n-4) for n>=9 */
    if (cache_size <= n) {
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 4];
        cache_size = n + 1;
    }

    return cache[n];
}
