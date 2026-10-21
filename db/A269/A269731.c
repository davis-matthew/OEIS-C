/*
 * A269731  Dimensions of the 3-polytridendriform operad TDendr_3.
 * https://oeis.org/A269731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+1)*a(n) = 7*(2*n-1)*a(n-1) - (n-2)*a(n-2) Initial values:
 *            a(1)=1, a(2)=7 (OEIS A269731)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation: cache[0] unused, cache[1] = a(1), cache[2] = a(2) */
    static seq_t *cache = NULL;
    static idx_t max_comp = 0;     /* highest index already computed      */
    static size_t cache_size = 0;  /* allocated number of cache slots     */

    /* Allocate/resize cache lazily on first call or when needed */
    if (cache == NULL) {
        cache_size = (n > 2) ? n + 1 : 3;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory, unlikely in tests */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        max_comp = 2;
    }
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    while (max_comp < n) {
        idx_t i = max_comp + 1;      /* next index to compute */

        seq_t ai;
        if (sizeof(seq_t) <= 8) { /* seq_t is 64‑bit (or smaller) – use 128‑bit intermediate */
            unsigned __int128 term1 = (unsigned __int128)cache[i-1] *
                                      (unsigned __int128)(2 * i - 1) * (unsigned __int128)7;
            unsigned __int128 term2 = (unsigned __int128)cache[i-2] *
                                      (unsigned __int128)(i - 2);
            unsigned __int128 num    = term1 - term2;
            ai = (seq_t)(num / (unsigned __int128)(i + 1));
        } else {              /* seq_t supports arbitrary precision or 128‑bit */
            seq_t term1 = cache[i-1];
            term1 *= (seq_t)(2 * i - 1);
            term1 *= (seq_t)7;

            seq_t term2 = cache[i-2];
            term2 *= (seq_t)(i - 2);

            seq_t num = term1 - term2;
            ai = num / (seq_t)(i + 1);
        }

        cache[i] = ai;
        max_comp = i;
    }

    return cache[n];
}
