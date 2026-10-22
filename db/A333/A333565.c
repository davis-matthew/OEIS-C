/*
 * A333565  O.g.f.: (1 + 4*x)/((1 + x)*sqrt(1 - 8*x)).
 * https://oeis.org/A333565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the P-recursive relation n*(3*n-4)*a(n) =
 *            (21*n^2-40*n+12)*a(n-1) +4*(3*n-1)*(2*n-3)*a(n-2), with a(0)=1,
 *            a(1)=7. This recurrence is implemented with exact division. For
 *            64‑bit builds a 128‑bit intermediate is used to avoid overflow,
 *            while for arbitrary‑precision builds the normal...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* static cache to support arbitrary‑order calls */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t  cap   = 0;         /* allocated capacity   */
    static idx_t  len   = 0;         /* number of cached values */

    /* Ensure that the cache is large enough for indices up to n */
    if ((idx_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;           /* a(0) = 1 */
        } else if (i == 1) {
            cache[1] = 7;           /* a(1) = 7 */
        } else {
#ifdef __cplusplus
            /* Arbitrary‑precision (C++) path – rely on operator overloading */
            seq_t term1 = (seq_t)(21*i*i - 40*i + 12);
            seq_t term2 = (seq_t)4 * (seq_t)(3*i - 1) * (seq_t)(2*i - 3);
            seq_t numerator = term1 * cache[i-1] + term2 * cache[i-2];
            seq_t denom     = (seq_t)i * (seq_t)(3*i - 4);
            cache[i] = numerator / denom;
#else
            /* 64‑bit (C) path – use 128‑bit intermediate */
            unsigned __int128 t1 = (unsigned __int128)21 * i * i
                                 - (unsigned __int128)40 * i + 12;
            unsigned __int128 t2 = (unsigned __int128)4
                                 * ((unsigned __int128)3 * i - 1)
                                 * ((unsigned __int128)2 * i - 3);
            unsigned __int128 num = t1 * (unsigned __int128)cache[i-1]
                                  + t2 * (unsigned __int128)cache[i-2];
            unsigned __int128 den = (unsigned __int128)i
                                 * ((unsigned __int128)3 * i - 4);
            cache[i] = (seq_t)(num / den);
#endif
        }
    }
    len = n + 1;
    return cache[n];
}
