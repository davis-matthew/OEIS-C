/*
 * A352735  Lucky numbers in Chinese: Numbers whose decimal expansion contains
 *          8 but not 4.
 * https://oeis.org/A352735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal expansion
 *            contains * at least one digit 8 and contains no digit 4
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_valid_digits, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has_valid_digits.h"

/* Main sequence accessor.  Uses a static cache that grows as needed. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, 0 is undefined; return 0 */
        return 0;

    /* Static cache state */
    static seq_t *cache = NULL;    /* pointer to cached values */
    static size_t cache_len = 0;   /* number of cached elements */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static seq_t next_candidate = 8;   /* next value to test */

    /* Generate until we have at least n elements */
    while (cache_len < n) {
        seq_t x = next_candidate;

        /* Find the next number that satisfies the conditions */
        while (!has_valid_digits(x))
            ++x;

        /* Resize cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp;
            if (cache == NULL)
                tmp = seq_alloc(new_cap);
            else
                tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp)          /* allocation failure – in practice this should not happen */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the result and update state */
        cache[cache_len++] = x;
        next_candidate = x + 1;   /* start next search after the accepted number */
    }

    return cache[n - 1];
}
