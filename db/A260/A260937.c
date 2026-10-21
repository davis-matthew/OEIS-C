/*
 * A260937  Indices i of pentagonal numbers P(i) such that (P(i)-1)/2 is also
 *          a pentagonal number.
 * https://oeis.org/A260937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A260937
   Indices i of pentagonal numbers P(i) such that (P(i)-1)/2 is also a pentagonal number.
   The sequence satisfies the linear recurrence
        a(1)=1,  a(2)=106,  a(3)=345,  a(4)=122130,  a(5)=397937
        a(n)= a(n‑1)+1154*a(n‑2)-1154*a(n‑3)-a(n‑4)+a(n‑5)  for n≥6
   (this follows from the generating function in the OEIS entry).
   The function caches computed values to answer arbitrary queries fast.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[0] is unused, cache[1]..cache[cache_size] are stored terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first five terms if not yet done */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – return 0 */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)106;
        cache[3] = (seq_t)345;
        cache[4] = (seq_t)122130;
        cache[5] = (seq_t)397937;
    }

    /* Grow cache if the requested index is beyond the current range */
    if (n > cache_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* reallocation failed – return 0 */
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = (seq_t)1154 * cache[i-2];
            seq_t t3 = (seq_t)1154 * cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            cache[i] = t1 + t2 - t3 - t4 + t5;
        }
        cache_size = n;
    }

    return cache[n];
}
