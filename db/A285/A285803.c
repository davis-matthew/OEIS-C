/*
 * A285803  Numbers k such that both k and 2k contain the digit 7.
 * https://oeis.org/A285803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   contains_digit7, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285803: numbers k such that both k and 2k contain digit 7.
   Computed via a linear search with caching. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc(seq_t*,size_t,size_t) */
#include "../utils/contains_digit7.h"

/* Static cache for generated sequence values. */
static seq_t *cache      = NULL;   /* pointer to the cached array */
static size_t  cached_len = 0;     /* number of values currently cached */
static size_t  cached_cap = 0;     /* capacity of the cache array */
static seq_t  next_candidate = 1;  /* next integer to examine */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0, offset is 1 */

    /* Extend the cache until it contains at least n terms. */
    while (cached_len < n)
    {
        /* Test the current candidate. */
        if (contains_digit7(next_candidate))
        {
            seq_t double_val = next_candidate + next_candidate;
            if (contains_digit7(double_val))
            {
                /* Grow the cache if needed. */
                if (cached_len == cached_cap)
                {
                    size_t new_cap = cached_cap ? cached_cap << 1 : 1;
                    seq_t *tmp = seq_realloc(cache, cached_cap, new_cap);
                    if (!tmp)
                        abort();   /* memory allocation failed */
                    cache = tmp;
                    cached_cap = new_cap;
                }
                cache[cached_len++] = next_candidate;   /* store the found value */
            }
        }
        ++next_candidate;   /* proceed to the next integer */
    }

    return cache[n - 1];   /* return the nth term (1‑based index) */
}
