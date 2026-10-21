/*
 * A289799  p-INVERT of (n^3), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 27*a(n-2) + 55*a(n-3) - 36*a(n-4) + 55*a(n-5)
 *            - 27*a(n-6) + 9*a(n-7) - a(n-8), with a(0)=1, a(1)=10, a(2)=62,
 *            a(3)=377, a(4)=2232, a(5)=13015, a(6)=75898, a(7)=444014
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of terms stored, i.e. cache length */
    static size_t cap = 0;   /* current allocated length */

    /* Base values for n = 0 .. 7 */
    static const seq_t base[8] = {
        1, 10, 62, 377,
        2232, 13015, 75898, 444014
    };

    /* Fast return for early terms */
    if (n < 8)
        return base[(size_t)n];

    /* First call: allocate cache and store base values */
    if (!cache) {
        cap  = 16;                      /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = base[i];
        len = 8;                        /* entries 0‑7 are valid */
    }

    /* Expand the cache if we need to store more terms */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap;
        while (newcap < need)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;            /* allocation failure – unlikely */
        cache = tmp;
        cap = newcap;
    }

    /* Compute any missing terms up to n */
    if (len <= n) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t s1 = 9  * cache[i-1];
            s1     += 55 * cache[i-3];
            s1     += 55 * cache[i-5];
            s1     += 9  * cache[i-7];

            seq_t s2 = 27 * cache[i-2];
            s2     += 36 * cache[i-4];
            s2     += 27 * cache[i-6];
            s2     += cache[i-8];

            cache[i] = s1 - s2;        /* recurrence */
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
