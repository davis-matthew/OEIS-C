/*
 * A368789  a(n) = (n+1)^2 * (n!)^3 * Sum_{k=1..n} 1/((k+1)^2 * (k!)^3).
 * https://oeis.org/A368789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n)= (n+1)^2 * n * a(n-1) + 1 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (0‑indexed) */
    static seq_t *cache = NULL;   /* array of computed values     */
    static size_t  len   = 0;      /* number of elements stored    */
    static size_t  cap   = 0;      /* allocated capacity           */

    /* handle base case immediately */
    if (n == 0)
        return 0;

    /* if requested index is beyond current capacity, grow array */
    if ((size_t)n + 1 > cap) {
        size_t needed = (size_t)n + 1;
        if (needed < 64) needed = 64;               /* small‑size start */
        while (cap < needed) {
            size_t new_cap = cap ? cap << 1 : 64;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc((void *)cache,
                                           new_cap * sizeof(seq_t));
            if (!tmp) abort();                      /* out‑of‑memory */
            cache = tmp;
            cap   = new_cap;
        }
    }

    /* initialise the first element if cache is empty */
    if (len == 0) {
        cache[0] = 0;
        len = 1;
    }

    /* iteratively compute terms up to n (inclusive) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t factor = ((seq_t)(i + 1) * (seq_t)(i + 1)) * (seq_t)i;
        cache[i] = factor * cache[i - 1] + 1;
    }

    len = (size_t)n + 1;
    return cache[n];
}
