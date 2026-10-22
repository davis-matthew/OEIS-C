/*
 * A378296  Number of cyclic edge cuts in the n-ladder graph.
 * https://oeis.org/A378296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence * a(n) = 30*a(n-1) -
 *            348*a(n-2) + 1936*a(n-3) * - 5168*a(n-4) + 5856*a(n-5) -
 *            2880*a(n-6) + 512*a(n-7) * with initial values a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=1, a(5)=32, * a(6)=612, a(7)=9160
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

#ifndef MAX_A_CACHE
#  define MAX_A_CACHE 10000  /* sufficient for n up to 10000 */
#endif

seq_t a(idx_t n)
{
    /* cache[0] is unused; indices 1..MAX_A_CACHE are valid */
    static seq_t cache[MAX_A_CACHE + 1] = {0};
    static idx_t cache_n = 0;   /* highest index computed so far */
    static int initialized = 0;

    /* initialise for known first terms */
    if (!initialized) {
        if (MAX_A_CACHE >= 1) cache[1] = (seq_t)0;
        if (MAX_A_CACHE >= 2) cache[2] = (seq_t)0;
        if (MAX_A_CACHE >= 3) cache[3] = (seq_t)0;
        if (MAX_A_CACHE >= 4) cache[4] = (seq_t)1;
        if (MAX_A_CACHE >= 5) cache[5] = (seq_t)32;
        if (MAX_A_CACHE >= 6) cache[6] = (seq_t)612;
        if (MAX_A_CACHE >= 7) cache[7] = (seq_t)9160;
        cache_n = 7;
        initialized = 1;
    }

    if (n <= cache_n)
        return cache[n];

    /* ensure we do not exceed the preallocated cache */
    if (n > MAX_A_CACHE) {
        /* fallback: compute iteratively without full storage */
        /* keep a ring buffer of the last 7 values */
        seq_t ring[7];
        /* initialise ring with last 7 computed values */
        for (idx_t i = 0; i < 7; ++i)
            ring[i] = cache[cache_n - 6 + i];
        idx_t curr = cache_n + 1;
        for (; curr <= n; ++curr) {
            seq_t next = 30 * ring[6]
                       - 348 * ring[5]
                       + 1936 * ring[4]
                       - 5168 * ring[3]
                       + 5856 * ring[2]
                       - 2880 * ring[1]
                       + 512  * ring[0];
            /* shift ring */
            for (int i = 0; i < 6; ++i)
                ring[i] = ring[i + 1];
            ring[6] = next;
        }
        return ring[6];
    }

    /* compute up to n using the cache */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        cache[i] = 30 * cache[i-1]
                 - 348 * cache[i-2]
                 + 1936 * cache[i-3]
                 - 5168 * cache[i-4]
                 + 5856 * cache[i-5]
                 - 2880 * cache[i-6]
                 + 512  * cache[i-7];
    }
    cache_n = n;
    return cache[n];
}
