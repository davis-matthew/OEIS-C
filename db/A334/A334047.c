/*
 * A334047  a(n) is the number of tilings of a bracelet of length 2n with 1
 *          color of 5-minoes and 6-minoes, 2 colors of 7-minoes and 8-minoes,
 *          3 colors of 9-minoes and 10-minoes, and so on.
 * https://oeis.org/A334047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334047:
   a(1)=0, a(2)=0, a(3)=6, a(4)=16, a(5)=35, a(6)=78
   a(n) = 4*a(n-1) - 6*a(n-2) + 6*a(n-3)
          - 5*a(n-4) + 3*a(n-5) - a(n-6)   for n>6
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i>=1; cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity (array size)   */
    static idx_t  maxc  = 0;    /* highest index computed */

    /* ------- ensure capacity for indices up to n -------- */
    if (n + 1 > cap) {
        size_t newcap = (cap > 0) ? cap * 2 : 16;
        while (newcap <= n)   /* need space for n+1 elements */
            newcap <<= 1;     /* double until sufficient */

        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t*) malloc(newcap * sizeof(seq_t));
        } else {
            tmp = (seq_t*) realloc(cache, newcap * sizeof(seq_t));
        }
        if (tmp == NULL)          /* allocation failed – silence caller */
            return 0;

        /* initialize newly allocated slots to 0 */
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = 0;

        cache = tmp;
        cap   = newcap;
    }

    /* ------- initialize base values once ------- */
    if (maxc == 0) {
        /* make sure cache has room for indices 1..6 */
        if (cap < 7) {
            size_t newcap = 7;
            seq_t *tmp = (seq_t*) realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;
            for (size_t i = cap; i < newcap; ++i)
                tmp[i] = 0;
            cache = tmp;
            cap   = newcap;
        }

        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 6;
        cache[4] = 16;
        cache[5] = 35;
        cache[6] = 78;
        maxc = 6;
    }

    /* ------- compute any missing terms up to n ------- */
    if (n > maxc) {
        for (idx_t i = maxc + 1; i <= n; ++i) {
            cache[i] = (4 * cache[i-1])               /* 4*a(n-1) */
                       - (6 * cache[i-2])              /* -6*a(n-2) */
                       + (6 * cache[i-3])              /* +6*a(n-3) */
                       - (5 * cache[i-4])              /* -5*a(n-4) */
                       + (3 * cache[i-5])              /* +3*a(n-5) */
                       - cache[i-6];                   /* -a(n-6)  */
        }
        maxc = n;
    }

    return cache[n];
}
