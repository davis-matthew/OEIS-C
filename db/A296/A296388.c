/*
 * A296388  a(1) = a(3) = 1, a(2) = a(4) = 3, a(5) = 4; a(n) = n - a(n-a(n-1))
 *          - a(n-a(n-4)) for n > 5.
 * https://oeis.org/A296388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - a(n - a(n-1)) - a(n - a(n-4)) Initial values: a(1)=1,
 *            a(2)=3, a(3)=1, a(4)=3, a(5)=4 Recurrence is valid for n>5 The
 *            function uses a static cache so calls can come in any order
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (array size) */
    static idx_t  cache_size = 0;  /* highest index that has been computed */

    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Allocate or grow the cache so that it has space up to index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);              /* allocation failed */
        if (new_cap > cache_cap) {
            /* zero the newly allocated part */
            size_t old_cap = cache_cap;
            memset(new_cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1 || i == 3) {
            cache[i] = (seq_t)1;
        } else if (i == 2 || i == 4) {
            cache[i] = (seq_t)3;
        } else if (i == 5) {
            cache[i] = (seq_t)4;
        } else {
            seq_t a_i_1 = cache[i - 1];
            seq_t a_i_4 = cache[i - 4];
            idx_t j1 = i - (idx_t)a_i_1;
            idx_t j2 = i - (idx_t)a_i_4;
            cache[i] = (seq_t)i - cache[j1] - cache[j2];
        }
    }
    cache_size = n;
    return cache[n];
}
