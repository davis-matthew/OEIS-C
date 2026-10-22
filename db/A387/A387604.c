/*
 * A387604  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 3^(n-2*k) *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A387604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A387604
 * Formula: a(n) = 6*a(n-1) - 9*a(n-2) + 6*a(n-3) + 18*a(n-4) - 9*a(n-6)
 * Initial values:
 *   a(0)=1, a(1)=6, a(2)=27, a(3)=114, a(4)=495, a(5)=2214
 * The function keeps a static cache so it can answer calls in any order.
 */

#include <stdlib.h>

/* External linkage function requested by the build */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t max_computed = 0;/* last computed index + 1 */

    /* if already computed, return it */
    if (n < max_computed)
        return cache[n];

    /* expand cache if necessary */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        size_t new_cap = (size_t)needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – unlikely for small n */
            return (seq_t)0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms */
    for (idx_t i = max_computed; i < needed; ++i) {
        if (i < 6) {
            static const seq_t init[6] = {
                (seq_t)1, (seq_t)6, (seq_t)27,
                (seq_t)114, (seq_t)495, (seq_t)2214
            };
            cache[i] = init[i];
        } else {
            seq_t term = 6 * cache[i - 1];
            term += 6 * cache[i - 3];
            term += 18 * cache[i - 4];

            seq_t sub = 9 * cache[i - 2];
            sub += 9 * cache[i - 6];

            cache[i] = term - sub;
        }
    }

    max_computed = needed;
    return cache[n];
}
