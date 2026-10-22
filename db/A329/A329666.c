/*
 * A329666  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UU and HH.
 * https://oeis.org/A329666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = a(n-2) + a(n-3) + Sum_{k=0..n-3} a(k) * a(n-3-k) with
 *            a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 61, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* lazily allocated cache for computed values */
    static seq_t *cache = NULL;     /* array of a(i) */
    static size_t cap   = 0;       /* allocated length */
    static idx_t  size  = 0;       /* number of values stored (highest index+1) */

    /* grow capacity if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute values up to n */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            /* small indices have fixed value */
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = (seq_t)1;
            } else {
#ifdef __cplusplus
                /* C++ build – arbitrary‑precision seq_t available */
                seq_t sum = 0;
                for (idx_t k = 0; k <= i-3; ++k) {
                    sum += cache[k] * cache[i-3-k];
                }
                cache[i] = cache[i-2] + cache[i-3] + sum;
#else
                /* C build – use 128‑bit intermediate to avoid overflow */
                unsigned __int128 sum = 0;
                for (idx_t k = 0; k <= i-3; ++k) {
                    sum += (unsigned __int128)cache[k] *
                           (unsigned __int128)cache[i-3-k];
                }
                cache[i] = cache[i-2] + cache[i-3] + (seq_t)sum;
#endif
            }
        }
        size = n + 1;
    }

    return cache[n];
}
