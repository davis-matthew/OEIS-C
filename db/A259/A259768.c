/*
 * A259768  Number of (n+2)X(4+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 or 00000101.
 * https://oeis.org/A259768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-7) +5*a(n-10)+a(n-13)-a(n-14)-a(n-17)-2*a(n-21) for
 *            n > 24; initial values are a(1)=201, a(2)=299, …, a(24)=49942
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 71, u64 n <= 164, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Ensure capacity for up to n elements. */
    if (cache_cap < n) {
        idx_t new_cap = (n > 24) ? n : 24;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure */
        if (!cache) { /* first allocation: copy the initial 24 terms */
            static const seq_t init[24] = {
                201ULL, 299ULL, 385ULL, 451ULL, 474ULL, 835ULL,
                872ULL, 968ULL, 1532ULL, 1794ULL, 2116ULL, 2746ULL,
                4218ULL, 4546ULL, 5038ULL, 8578ULL, 9510ULL,
                10852ULL, 15782ULL, 21068ULL, 23626ULL, 28628ULL,
                45684ULL, 49942ULL
            };
            for (idx_t i = 0; i < 24; ++i) tmp[i] = init[i];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure that the first 24 terms are marked as computed. */
    if (cache_len < 24) cache_len = 24;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1; /* zero‑based index in cache */
        cache[idx] =
            3 * cache[idx - 7] +
            5 * cache[idx -10] +
              cache[idx -13] -
              cache[idx -14] -
              cache[idx -17] -
            2 * cache[idx -21];
    }

    cache_len = n;
    return cache[n - 1];
}
