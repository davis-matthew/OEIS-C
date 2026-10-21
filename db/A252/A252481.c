/*
 * A252481  Numbers whose set of digits is simply connected to 1.
 * https://oeis.org/A252481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_simply_connected_to_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th positive integer whose decimal digit set is a
   contiguous block of digits that contains the digit 1.
   Implemented by incremental search with memoisation. */

#include <stdlib.h>
#include "../utils/is_simply_connected_to_one.h"

static seq_t *cache = NULL;     /* array of previously computed terms    */
static size_t cache_cap = 0;    /* current capacity of cache             */
static idx_t cache_len = 0;     /* number of terms currently stored       */
static seq_t last_val = 0;      /* last term that was generated          */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, but not used */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until we reach the nth term */
    seq_t cur = last_val + 1;

    while (cache_len < n) {
        if (is_simply_connected_to_one(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        cur += 1;
    }

    last_val = cur - 1;
    return cache[n - 1];
}
