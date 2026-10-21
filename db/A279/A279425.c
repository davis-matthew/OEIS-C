/*
 * A279425  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 5.
 * https://oeis.org/A279425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 2803, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = k such that k^2 has an odd number of digits and its middle digit is 5 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t  next_k      = 1;   /* next candidate to test */

    if (n == 0) {
        /* Undefined for n < 1; return 0 */
        return (seq_t)0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k = next_k;
        seq_t sq = k * k;                      /* square of the candidate */

        int d = digit_len(sq);                 /* number of decimal digits */
        if ((d & 1) != 0) {                    /* only odd lengths */
            seq_t mid = (sq / pow10((idx_t)(d / 2))) % (seq_t)10;
            if (mid == (seq_t)5) {             /* middle digit is 5 */
                /* Ensure capacity in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failure */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;        /* store the term */
            }
        }
        ++next_k;                               /* advance to next candidate */
    }

    return cache[n - 1];
}
