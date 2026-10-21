/*
 * A295017  Squares whose largest digit is 7.
 * https://oeis.org/A295017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_max_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295017: Squares whose largest digit is 7.
   a(n) = [the n-th integer whose square has maximum decimal digit 7]^2
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_max_digit.h"

/* Expanding cache of computed squares. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of elements stored */
static idx_t  cache_cap = 0;   /* capacity of the cache array */
static idx_t  next_root = 1;   /* next integer to test */

/* Ensure the cache has at least 'needed' elements. */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* The OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; return 0 as placeholder */

    if (n <= cache_len)            /* already computed */
        return cache[n-1];

    /* Compute until we have n terms. */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_root * (seq_t)next_root;
        if (has_max_digit(sq)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = sq;
        }
        next_root++;
    }
    return cache[n-1];
}
