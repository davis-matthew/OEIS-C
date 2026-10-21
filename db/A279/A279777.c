/*
 * A279777  Numbers k such that the sum of digits of 9k is 27.
 * https://oeis.org/A279777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279777:
   a(n) = the nth positive integer k such that
          sum of decimal digits of 9*k equals 27. */

#include <stdint.h>
#include <stdlib.h>
/* Use the provided utility for digit sums */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;   /* array of found k's      */
    static size_t cache_cap  = 0;      /* capacity of array       */
    static size_t cache_len  = 0;      /* number of stored k's    */
    static idx_t  next_k      = 1;     /* next candidate to test  */

    /* On first call, allocate a small cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);   /* fatal if we can't allocate */
    }

    /* If the requested term is already in the cache, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;          /* candidate k                */
        seq_t n9  = k * (seq_t)9;           /* 9*k                        */
        if (digit_sum(n9) == (seq_t)27) {   /* test the condition         */
            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *newcache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (newcache == NULL) exit(1);
                cache = newcache;
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
