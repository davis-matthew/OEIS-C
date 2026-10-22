/*
 * A302517  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 4 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+5*a(n-4)-a(n-5)-5*a(n-6)-4*a(n-7)
 *            for n > 10 with initial values: a(1)=8, a(2)=6, a(3)=9, a(4)=28,
 *            a(5)=36, a(6)=80, a(7)=235, a(8)=541, a(9)=1115, a(10)=2554
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* return 0 for indices below the offset (not used in OEIS) */
    if (n == 0) return (seq_t)0;

    /* static cache for memoization */
    static seq_t *cache = NULL;       /* cache[1] .. cache[last] */
    static idx_t last = 0;            /* number of terms computed so far */
    static idx_t capacity = 0;        /* allocated size */

    /* pre‑defined initial values */
    static const seq_t init[11] = {
        (seq_t)0,               /* dummy for index 0 */
        (seq_t)8, (seq_t)6, (seq_t)9, (seq_t)28,
        (seq_t)36, (seq_t)80, (seq_t)235,
        (seq_t)541, (seq_t)1115, (seq_t)2554
    };

    /* allocate cache if needed */
    if (cache == NULL) {
        capacity = 64;                       /* arbitrary starting size */
        cache = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
    }

    /* grow cache to hold up to index n */
    if (n > capacity) {
        capacity = n * 2;
        cache = (seq_t *)realloc(cache, (capacity + 1) * sizeof(seq_t));
    }

    /* if value already computed, return it */
    if (n <= last) {
        return cache[n];
    }

    /* fill initial terms if not yet done */
    if (last < 10) {
        for (idx_t i = last + 1; i <= 10; ++i) {
            cache[i] = init[i];
        }
        last = 10;
    }

    /* compute remaining terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[i-1]
                  + cache[i-2]
                  + (seq_t)3 * cache[i-3]
                  + (seq_t)5 * cache[i-4]
                  - cache[i-5]
                  - (seq_t)5 * cache[i-6]
                  - (seq_t)4 * cache[i-7];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
