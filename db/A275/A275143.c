/*
 * A275143  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n>5 Initial values:
 *            a(1)=5, a(2)=36, a(3)=80, a(4)=224, a(5)=528
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index that has been computed */

    if (n == (idx_t)0) return 0;   /* offset behaviour is irrelevant */

    /* Extend cache if necessary */
    if (cache_size < n) {
        idx_t new_size = n + 1;    /* allocate space for indices 0..n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)5;   break;
                case 2: cache[2] = (seq_t)36;  break;
                case 3: cache[3] = (seq_t)80;  break;
                case 4: cache[4] = (seq_t)224; break;
                case 5: cache[5] = (seq_t)528; break;
                default:
                    /* Recurrence: a(i) = 3*a(i-1) - 2*a(i-2) + a(i-3) */
                    cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];
                    break;
            }
        }
        cache_size = n;
    }
    return cache[n];
}
