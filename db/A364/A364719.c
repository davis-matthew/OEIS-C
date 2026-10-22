/*
 * A364719  Numbers k such that d(k) > d(k+1) > d(k+2) > d(k+3), where d(n) is
 *          the number of divisors of n.
 * https://oeis.org/A364719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer k such that d(k)>d(k+1)>d(k+2)>d(k+3),
 *            where d(x) is the number of divisors of x. The implementation
 *            performs a linear search, caching all found terms so that
 *            subsequent calls are serviced in O(1)
 * Range:     u8 n <= 2, u16 n <= 708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t n) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of found k's */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  size   = 0;      /* number of stored terms */
    static idx_t   last   = 0;      /* last integer checked */

    /* Ensure the cache can hold n terms */
    while (size < n) {
        /* Advance to next candidate */
        if (last == ~((idx_t)0) - 3) break;   /* prevent overflow */
        last++;
        idx_t k = last;

        seq_t d0 = divisor_count_u64((uint64_t)k);
        seq_t d1 = divisor_count_u64((uint64_t)(k + 1));
        seq_t d2 = divisor_count_u64((uint64_t)(k + 2));
        seq_t d3 = divisor_count_u64((uint64_t)(k + 3));

        if (d0 > d1 && d1 > d2 && d2 > d3) {
            /* Resize cache if necessary */
            if (size >= cap) {
                size_t new_cap = cap < 16 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cap   = new_cap;
            }
            /* Store the found integer */
            cache[size++] = (seq_t)k;
        }
    }

    /* n is guaranteed to be >= 1, size >= n now */
    return cache[n - 1];
}
