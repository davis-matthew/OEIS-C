/*
 * A248428  Number of length n+2 0..3 arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 85, u64 n <= 181,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A248428
   Recurrence: a(1)=16, a(2)=20, a(3)=24, a(4)=28,
               a(n)=a(n-1)+2*a(n-3)-2*a(n-4)  (n≥5)
*/

#include <stdlib.h>

/* Use external integer types provided by the build system */
extern seq_t a(idx_t n);   /* forward declaration for type checking */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined below offset 1 */

    /* static cache to support arbitrary call ordering */
    static seq_t *cache = NULL;        /* cache[1..last] are valid */
    static size_t cap   = 0;           /* allocated capacity (bytes) */
    static size_t last  = 0;           /* highest index computed */

    /* target index as size_t */
    size_t target = (size_t)n;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 5;                     /* indices 0..4 (0 unused) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure -> 0 */

        cache[1] = (seq_t)16;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)28;
        last   = 4;
    }

    /* grow cache if necessary */
    if (target + 1 > cap) {
        size_t new_cap = cap;
        while (target + 1 > new_cap)
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;    /* allocation failure -> 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* return already computed value */
    if (target <= last)
        return cache[target];

    /* recurrence: a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) */
    seq_t two = (seq_t)2;
    for (size_t i = last + 1; i <= target; ++i) {
        cache[i] = cache[i - 1]
                 + two * cache[i - 3]
                 - two * cache[i - 4];
    }
    last = target;
    return cache[target];
}
