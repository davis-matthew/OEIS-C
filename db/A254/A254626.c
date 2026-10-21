/*
 * A254626  Indices of triangular numbers (A000217) that are also centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A254626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 18*a(n-2) - 18*a(n-3) - a(n-4) + a(n-5) Initial
 *            values: a(1)=1, a(2)=3, a(3)=23, a(4)=61, a(5)=421
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;   /* number of terms currently computed */

    if (n == 0) return 0;  /* undefined for n=0, but return 0 */

    /* ensure storage */
    if (n > cap) {
        cap = n;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;
        } else if (i == 2) {
            cache[1] = 3;
        } else if (i == 3) {
            cache[2] = 23;
        } else if (i == 4) {
            cache[3] = 61;
        } else if (i == 5) {
            cache[4] = 421;
        } else {
            /* recurrence: a(i)=a(i-1)+18*a(i-2)-18*a(i-3)-a(i-4)+a(i-5) */
            cache[i - 1] = cache[i - 2] + 18 * cache[i - 3]
                                 - 18 * cache[i - 4]
                                 -      cache[i - 5]
                                 +      cache[i - 6];
        }
    }
    len = n;
    return cache[n - 1];
}
