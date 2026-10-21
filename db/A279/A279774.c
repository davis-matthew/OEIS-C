/*
 * A279774  Numbers n such that the sum of digits of 4n equals 8.
 * https://oeis.org/A279774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th positive integer for which the sum of
   decimal digits of 4·n equals 8 (OEIS A279774). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t last_tested = 0;  /* largest integer examined so far */

    /* offset 1: return the first term for n==1 */
    if (n == 0)
        return 0;   /* undefined, but safe for any n==0 call */

    /* if the term is already cached, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* initialise cache on first use */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        last_tested++;

        /* compute 4 * last_tested and its digit sum */
        seq_t val = (seq_t)4 * (seq_t)last_tested;
        seq_t sum = digit_sum(val);

        if (sum == 8) {
            /* ensure room in the cache */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = (seq_t)last_tested;
            cache_len++;
        }
    }

    return cache[n - 1];
}
