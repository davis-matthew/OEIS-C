/*
 * A373231  Relative of Hofstadter Q-sequence: a(n) = 0 for n <= 0, a(n) = n
 *          for 1 <= n <= 11; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))
 *          for n > 11.
 * https://oeis.org/A373231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 0, a(n) = n for 1 <= n <= 11, a(n) =
 *            a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 11
 * Range:     u8 n <= 316, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;

    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* allocated size of cache          */
    static idx_t last = 0;  /* highest index computed            */

    /* initialise cache on first call */
    if (!cache) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;          /* a(0) = 0                     */
    }

    /* enlarge cache if necessary */
    if (n >= cap) {
        idx_t newcap = cap ? cap << 1 : 16;
        while (newcap <= n)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute all terms up to n that are still missing */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = (seq_t)i;
            } else {
                idx_t idx1 = (idx_t)cache[i-1];
                idx_t idx2 = (idx_t)cache[i-2];
                idx_t idx3 = (idx_t)cache[i-3];

                seq_t v1 = cache[(i > idx1) ? (i - idx1) : 0];
                seq_t v2 = cache[(i > idx2) ? (i - idx2) : 0];
                seq_t v3 = cache[(i > idx3) ? (i - idx3) : 0];

                cache[i] = v1 + v2 + v3;
            }
        }
        last = n;
    }

    return cache[n];
}
