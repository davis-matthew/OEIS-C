/*
 * A274213  Meta recurrence: a(0) = 1, a(1) = 2, a(2) = 3, a(n) = a(n -
 *          a(n-3)) + 3 for n > 2.
 * https://oeis.org/A274213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - a(n-3)) + 3, a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;

    if (n >= len) {
        idx_t needed = n + 1;
        if (cap < needed) {
            idx_t newcap = cap ? cap * 2 : 16;
            while (newcap < needed) newcap *= 2;
            seq_t *newcache = (seq_t*)malloc((size_t)newcap * sizeof(seq_t));
            if (!newcache) return (seq_t)0;   /* allocation failed */
            if (cache) {
                for (idx_t i = 0; i < len; ++i)
                    newcache[i] = cache[i];
                free(cache);
            }
            cache = newcache;
            cap   = newcap;
        }
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)2;
            else if (i == 2)
                cache[i] = (seq_t)3;
            else {
                idx_t j = i - (idx_t)cache[i-3];
                cache[i] = cache[j] + (seq_t)3;
            }
        }
        len = n + 1;
    }
    return cache[n];
}
