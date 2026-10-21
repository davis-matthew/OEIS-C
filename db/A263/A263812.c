/*
 * A263812  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change (+-,+-) 0,0 0,1 0,2 or 1,0.
 * https://oeis.org/A263812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 23*a(n-2) - 10*a(n-3) - a(n-4); Initial
 *            values (offset 1): a(1)=82, a(2)=948, a(3)=11305, a(4)=134028
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* highest index that has been computed */

    if (n == 0) return 0;   /* undefined for offset 1, but harmless */

    /* Expand cache if necessary */
    if ((size_t)n > cap) {
        size_t target = (size_t)n;
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap < target) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;

        /* Ensure initial values are present */
        if (cap < 1) cache[1] = (seq_t)82;
        if (cap < 2) cache[2] = (seq_t)948;
        if (cap < 3) cache[3] = (seq_t)11305;
        if (cap < 4) cache[4] = (seq_t)134028;

        /* Compute missing terms using the recurrence */
        size_t start = cap < 5 ? 5 : (size_t)cap + 1;
        for (size_t i = start; i <= new_cap; ++i) {
            cache[i] = (10 * cache[i - 1]) + (23 * cache[i - 2])
                       - (10 * cache[i - 3]) - cache[i - 4];
        }
        cap = new_cap;
    }

    return cache[(size_t)n];
}
