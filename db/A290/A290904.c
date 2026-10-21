/*
 * A290904  p-INVERT of the positive integers, where p(S) = 1 - 2*S^2.
 * https://oeis.org/A290904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) - a(n-4), a(0) = 0, a(1) =
 *            2, a(2) = 8, a(3) = 24
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t   cap    = 0;     /* allocated capacity */
    static size_t   len    = 0;     /* number of initialized terms */

    /* Helper to grow and compute up to index n */
    if ((size_t)n >= len) {
        size_t need = (size_t)n + 1;
        if (cap < need) {
            size_t newcap = cap ? cap << 1 : 16;
            if (newcap < need) newcap = need;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i == 0)           cache[i] = 0;
            else if (i == 1)      cache[i] = 2;
            else if (i == 2)      cache[i] = 8;
            else if (i == 3)      cache[i] = 24;
            else                  cache[i] = 4*cache[i-1]
                                      - 4*cache[i-2]
                                      + 4*cache[i-3]
                                      - cache[i-4];
        }
        len = need;
    }
    return cache[(size_t)n];
}
