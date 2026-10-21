/*
 * A277089  Pisot sequences L(6,15), S(6,15).
 * https://oeis.org/A277089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(a(n-1)^2 / a(n-2)), a(0) = 6, a(1) = 15. Computed
 *            iteratively with memoization
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for memoized terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last   = (idx_t)-1;  /* last index stored */

    /* initial capacity */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)15;
        last = 1;
    }

    /* grow cache if needed */
    if (n > (idx_t)(cap - 1)) {
        size_t new_cap = cap * 2 > (size_t)n + 1 ? cap * 2 : (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    if (n <= last) return cache[n];

    /* compute new terms iteratively */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute ceil(a(i-1)^2 / a(i-2)) using 128‑bit intermediate */
        unsigned __int128 prod = (unsigned __int128)cache[i-1] * (unsigned __int128)cache[i-1];
        unsigned __int128 num  = prod + (unsigned __int128)cache[i-2] - 1;
        cache[i] = (seq_t)(num / (unsigned __int128)cache[i-2]);
    }
    last = n;
    return cache[n];
}
