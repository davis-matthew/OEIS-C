/*
 * A374259  a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), with a(0)=4, a(1)=6, a(2)=20.
 * https://oeis.org/A374259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3); a(0)=4, a(1)=6, a(2)=20
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: array for all computed terms */
    static seq_t *cache = NULL;   /* cache[0 .. len-1] are initialized */
    static size_t  len   = 0;     /* number of initialized entries */
    static size_t  cap   = 0;     /* allocated capacity */

    /* ensure array holds indices up to n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap ? cap * 2 : 4;   /* grow factor, min 4 */
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base terms once */
    if (len == 0) {
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)20;
        len     = 3;
    }

    /* compute further terms up to n if needed */
    if (n + 1 > len) {
        size_t start = (len > 3) ? len : 3;   /* recurrence starts at index 3 */
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t t = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-2];
            t -= cache[i-3];
            cache[i] = t;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
