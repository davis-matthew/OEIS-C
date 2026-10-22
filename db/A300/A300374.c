/*
 * A300374  Number of nX2 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-2) + 2*a(n-3) + a(n-5) - 3*a(n-6) + a(n-7)
 * Initial values: a(1)=1, a(2)=3, a(3)=2, a(4)=10, a(5)=13, a(6)=42, a(7)=74
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* not used; safety */

    /* Static cache of computed terms, with current maximum index stored in cache_cap */
    static seq_t *cache = NULL;               /* cache[0] = a(1) */
    static idx_t cache_cap = 0;               /* number of terms stored */

    /* Extend the cache if we need to compute larger indices */
    if (n > cache_cap) {
        /* Allocate or grow the internal array */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        /* Fill the missing values in order */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            if (i <= 7) {                      /* initial terms */
                switch ((int)i) {
                    case 1: cache[i - 1] = (seq_t)1;  break;
                    case 2: cache[i - 1] = (seq_t)3;  break;
                    case 3: cache[i - 1] = (seq_t)2;  break;
                    case 4: cache[i - 1] = (seq_t)10; break;
                    case 5: cache[i - 1] = (seq_t)13; break;
                    case 6: cache[i - 1] = (seq_t)42; break;
                    case 7: cache[i - 1] = (seq_t)74; break;
                }
            } else {                            /* recurrence for i >= 8 */
                seq_t next = 4 * cache[i - 3];   /* 4*a(n-2) */
                next += 2 * cache[i - 4];       /* + 2*a(n-3) */
                next += cache[i - 6];           /* +   a(n-5) */
                next -= 3 * cache[i - 7];       /* - 3*a(n-6) */
                next += cache[i - 8];           /* +   a(n-7) */
                cache[i - 1] = next;
            }
        }
        cache_cap = n;                         /* record the highest computed index */
    }

    return cache[n - 1];                       /* return the requested term */
}
