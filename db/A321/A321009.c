/*
 * A321009  a(1)=2; thereafter a(n) is obtained by applying Eric Angelini's
 *          remove-repeated-digits map, x->A320486(x), to n*a(n-1), stopping
 *          when 0 is reached.
 * https://oeis.org/A321009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(1) = 2; a(n) = remove_repeated_digits( n * a(n-1) ) where
 *            remove_repeated_digits deletes every decimal digit that appears
 *            more than once in its argument. The recurrence is evaluated with
 *            memoization, computing all terms up to n on demand
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   remove_repeated_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/remove_repeated_digits.h"

static seq_t *cache = NULL;          /* cache[1] .. cache[cache_len] */
static size_t cache_len = 0;         /* highest n for which cache[n] is valid */
static size_t cache_cap = 0;         /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Ensure cache is initialized. */
    if (cache_len == 0) {
        size_t init_cap = 1;
        cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t)); /* cache[0] unused */
        if (!cache)
            return 0;   /* out of memory: return 0 as a safe default */
        cache_cap = init_cap;
        cache[0] = 0;
        cache[1] = 2;
        cache_len = 1;
    }

    /* Resize cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *newptr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!newptr)
            return 0;   /* out of memory */
        cache = newptr;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        seq_t prev = cache[k - 1];
        seq_t prod = (seq_t)k * prev;
        cache[k] = remove_repeated_digits(prod);
    }
    cache_len = n;

    return cache[n];
}
