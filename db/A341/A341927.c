/*
 * A341927  Bisection of the numerators of the convergents of
 *          cf(1,4,1,6,1,6,...,6,1).
 * https://oeis.org/A341927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A341927
   Formula: a(0) = 1, a(1) = 6, a(n) = 8*a(n-1) - a(n-2)   (n >= 2)
   The implementation uses memoization to allow arbitrary order of calls
   and provides an O(n) linear construction up to the requested n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache       = NULL;   /* dynamic array of sequence values */
    static idx_t cache_len   = 0;       /* number of values stored in cache */

    /* Ensure capacity to store up to index n */
    if (cache_len == 0 || cache_len <= n) {
        idx_t new_size = (cache_len == 0) ? (n >= 1 ? n + 1 : 2) : (n + 1);
        /* Use realloc for both initial allocation and expansion */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
    }

    /* Build missing sequence terms up to index n */
    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;            /* a(0) = 1 */
        } else if (cache_len == 1) {
            cache[1] = (seq_t)6;            /* a(1) = 6 */
        } else {
            /* a(k) = 8*a(k-1) - a(k-2) */
            cache[cache_len] = (seq_t)8 * cache[cache_len - 1] - cache[cache_len - 2];
        }
        ++cache_len;
    }

    return cache[n];
}
