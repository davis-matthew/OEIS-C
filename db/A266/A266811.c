/*
 * A266811  Total number of ON (black) cells after n iterations of the "Rule
 *          62" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-5)-a(n-7)+a(n-8) for n > 7, with
 *            a(0)=1, a(1)=4, a(2)=7, a(3)=13, a(4)=18, a(5)=26, a(6)=35,
 *            a(7)=45
 * Range:     u8 n <= 18, u16 n <= 322, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t old_cap = cache_cap;

    /* grow cache if needed (use helper that yields seq_t*) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_arr = alloc_seq_array(new_cap);
        if (!new_arr) return 0;                    /* allocation failure */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_arr[i] = cache[i];
            free_cache(cache);                     /* free old array */
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* compute values that are not yet cached */
    for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
        if (i == 0)           cache[0] = 1;
        else if (i == 1)      cache[1] = 4;
        else if (i == 2)      cache[2] = 7;
        else if (i == 3)      cache[3] = 13;
        else if (i == 4)      cache[4] = 18;
        else if (i == 5)      cache[5] = 26;
        else if (i == 6)      cache[6] = 35;
        else if (i == 7)      cache[7] = 45;
        else
            cache[i] = cache[i-1]         /* a(n-1) */
                     + cache[i-3]         /* a(n-3) */
                     - cache[i-5]         /* a(n-5) */
                     - cache[i-7]         /* a(n-7) */
                     + cache[i-8];        /* a(n-8) */
    }

    return cache[n];
}
