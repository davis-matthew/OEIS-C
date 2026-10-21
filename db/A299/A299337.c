/*
 * A299337  Expansion of 1 / ((1 - x)^7*(1 + x)^5).
 * https://oeis.org/A299337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 209, u64 n <= 8664,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299337 – Expansion of 1 / ((1 - x)^7 * (1 + x)^5)
 *
 *  Recurrence (for n > 11):
 *      a(n) = 2*a(n-1) + 4*a(n-2) - 10*a(n-3) - 5*a(n-4) +
 *             20*a(n-5) - 20*a(n-7) + 5*a(n-8) + 10*a(n-9) -
 *             4*a(n-10) - 2*a(n-11) + a(n-12)
 *
 *  Initial values (0‑based):
 *      a(0)=1, a(1)=2, a(2)=8, a(3)=14, a(4)=35, a(5)=56,
 *      a(6)=112, a(7)=168, a(8)=294, a(9)=420, a(10)=672,
 *      a(11)=924, a(12)=1386
 *
 *  This implementation memoises the values in a static array.
 *  The algorithm is O(n) per call for the largest n requested up
 *  to 10 000, well within the 2‑second limit on any reasonably
 *  sized machine.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[13] = {
        1,  2,  8, 14, 35, 56, 112, 168,
        294, 420, 672, 924, 1386
    };

    /* Static cache state */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static idx_t  maxseen = 12;     /* highest index computed so far */
    static size_t cap     = 13;     /* allocated capacity */

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
    }

    /* If needed, grow the array to hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* abort on failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute any missing values up to n */
    if (n > maxseen) {
        for (size_t i = maxseen + 1; i <= (size_t)n; ++i) {
            /* use signed __int128 for intermediate arithmetic
               to avoid overflow in the 64‑bit build */
            __int128 sum =
                  2   * (__int128)cache[i-1] +
                  4   * (__int128)cache[i-2]
              - 10  * (__int128)cache[i-3]
              - 5   * (__int128)cache[i-4]
              + 20  * (__int128)cache[i-5]
              - 20  * (__int128)cache[i-7]
              + 5   * (__int128)cache[i-8]
              + 10  * (__int128)cache[i-9]
              - 4   * (__int128)cache[i-10]
              - 2   * (__int128)cache[i-11]
              +     (__int128)cache[i-12];
            cache[i] = (seq_t)sum;
        }
        maxseen = n;
    }

    return cache[(size_t)n];
}
