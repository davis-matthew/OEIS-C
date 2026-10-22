/*
 * A336231  Integers whose binary digit expansion has an even number of 0's
 *          between any two consecutive 1's.
 * https://oeis.org/A336231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose binary digits contain only even
 *            runs of zeros between 1s (computed by simple enumeration)
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Check whether a number satisfies the property. */
static bool is_valid(seq_t x)
{
    seq_t y = x;
    bool seen = false;      /* have we seen the first 1 yet? */
    idx_t zeros = 0;        /* zeros counted since last 1 */

    while (y != 0) {
        if (y & 1) {                 /* a 1 bit */
            if (seen && (zeros & 1)) /* odd zeros since previous 1 */
                return false;
            seen = true;
            zeros = 0;
        } else {                     /* a 0 bit */
            if (seen)
                zeros++;
        }
        y >>= 1;
    }
    return true;
}

/* Return the n-th term of A336231. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t current_scan = 0;

    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        current_scan = 0;
    }

    while (cache_len < n) {
        if (is_valid(current_scan)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = current_scan;
        }
        current_scan++;
    }

    return cache[n - 1];
}
