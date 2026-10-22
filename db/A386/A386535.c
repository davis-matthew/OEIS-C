/*
 * A386535  Number of independent vertex sets in the n-Lindgren-Sousselier
 *          graph.
 * https://oeis.org/A386535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 84*a(n-2) - 87*a(n-3) - 13*a(n-4) + 4*a(n-5)
 *            with initial values: a(0)=7, a(1)=76, a(2)=997, a(3)=13965,
 *            a(4)=202828
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: stored values from a(0) up to a(used-1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of the cache array */
    static size_t used  = 0;   /* number of values currently computed */

    /* Ensure the cache can hold at least n+1 entries */
    if (n >= used) {
        size_t needed = n + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }
        /* Compute new terms up to n */
        for (size_t i = used; i <= (size_t)n; ++i) {
            switch (i) {
                case 0: cache[i] = 7; break;
                case 1: cache[i] = 76; break;
                case 2: cache[i] = 997; break;
                case 3: cache[i] = 13965; break;
                case 4: cache[i] = 202828; break;
                default:
                    cache[i] = 21 * cache[i-1]
                               - 84 * cache[i-2]
                               - 87 * cache[i-3]
                               - 13 * cache[i-4]
                               + 4  * cache[i-5];
            }
        }
        used = n + 1;
    }
    return cache[n];
}
