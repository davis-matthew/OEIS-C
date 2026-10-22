/*
 * A303219  a(n) = n for n <= 11. Thereafter a(n) = a(n-a(n-2)) + a(n-a(n-4))
 *          + a(n-a(n-6)).
 * https://oeis.org/A303219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n <= 11; a(n) = a(n-a(n-2)) + a(n-a(n-4)) +
 *            a(n-a(n-6)) for n > 11
 * Range:     u8 n <= 369, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size   */
    static idx_t  max    = 0;    /* largest n computed */

    /* grow cache if necessary */
    if ((idx_t)cap < n) {
        size_t new_cap = cap * 2;
        if (new_cap < 4) new_cap = 4;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed, return 0 */
            return (seq_t)0;
        }
        if (cache) {
            memcpy(new_cache, cache, max * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* already computed? */
    if (n <= max) {
        return cache[n-1];
    }

    /* compute missing values up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        if (i <= 11u) {
            cache[i-1] = (seq_t)i;            /* base case */
        } else {
            /* values of a(i-2), a(i-4), a(i-6) are already known */
            seq_t m2 = cache[i-3];            /* a(i-2)  */
            seq_t m4 = cache[i-5];            /* a(i-4)  */
            seq_t m6 = cache[i-7];            /* a(i-6)  */

            /* compute indices of the recursive calls */
            idx_t idx1 = (idx_t)(i - (idx_t)m2);
            idx_t idx2 = (idx_t)(i - (idx_t)m4);
            idx_t idx3 = (idx_t)(i - (idx_t)m6);

            /* recursive definition */
            cache[i-1] = cache[idx1-1] + cache[idx2-1] + cache[idx3-1];
        }
    }

    max = n;
    return cache[n-1];
}
