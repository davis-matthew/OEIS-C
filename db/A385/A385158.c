/*
 * A385158  Numbers k such that the sum of the digits of k is a number that
 *          appears as a substring of k, and every nonzero digit of k appears
 *          in that sum.
 * https://oeis.org/A385158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_sum_substring_and_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers k in ascending order and test
   the property: digit sum of k appears as a substring of k and
   every nonzero digit of k appears in that sum.  Store found
   numbers in a cache and return the n‑th term. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_sum_substring_and_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 0 */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_k = 0;   /* last k checked or stored */

    if (n <= cache_len)
        return cache[n - 1];

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    seq_t k = last_k + (seq_t)1;
    while (cache_len < n) {
        if (has_sum_substring_and_digits(k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }
        ++k;
    }
    last_k = k - (seq_t)1;
    return cache[n - 1];
}
