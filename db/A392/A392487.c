/*
 * A392487  a(n) = Sum_{k=0..floor((2*n+1)/5)} binomial(2*k+1,2*n-5*k+1).
 * https://oeis.org/A392487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) + a(n-5) - 2*a(n-6) + a(n-7); Initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=3, a(4)=0, a(5)=5, a(6)=10
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 97, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_cap = 0;

    /* extend cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0; /* allocation failure */

        static const seq_t init[7] = {
            (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)3,
            (seq_t)0, (seq_t)5, (seq_t)10
        };

        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = 4*a(i-3) + a(i-5) - 2*a(i-6) + a(i-7) */
                seq_t t1 = cache[i-3] * (seq_t)4;
                seq_t t2 = cache[i-5];
                seq_t t3 = cache[i-7];
                seq_t t4 = cache[i-6] * (seq_t)2;
                cache[i] = t1 + t2 + t3 - t4;
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
