/*
 * A384611  Expansion of (1+2*x-x^3) / (1-5*x^2+2*x^4).
 * https://oeis.org/A384611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=5, a(3)=9;
   for n≥4:  a(n) = 5*a(n-2) – 2*a(n-4)  */
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* static cache */
#ifdef __cplusplus
    static seq_t *cache = nullptr;
    static size_t cap   = 0;
#else
    static seq_t *cache = NULL;
    static size_t cap   = 0;
#endif

    if ((size_t)n >= cap) {
        /* enlarge cache: grow multiplicatively */
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;

#ifdef __cplusplus
        /* allocate new array of seq_t objects */
        seq_t *tmp = new seq_t[new_cap];
        if (cap) {
            /* copy already computed values */
            std::copy(cache, cache + cap, tmp);
            delete[] cache;
        }
        cache = tmp;
        /* compute new entries */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0)           cache[i] = seq_t(1);
            else if (i == 1)      cache[i] = seq_t(2);
            else if (i == 2)      cache[i] = seq_t(5);
            else if (i == 3)      cache[i] = seq_t(9);
            else                  cache[i] = (seq_t)5 * cache[i-2] - (seq_t)2 * cache[i-4];
        }
        cap = new_cap;
#else
        /* allocate new array of raw objects */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        /* compute new entries */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0)           cache[i] = 1;
            else if (i == 1)      cache[i] = 2;
            else if (i == 2)      cache[i] = 5;
            else if (i == 3)      cache[i] = 9;
            else                  cache[i] = (seq_t)5 * cache[i-2] - (seq_t)2 * cache[i-4];
        }
        cap = new_cap;
#endif
    }
    return cache[(size_t)n];
}
