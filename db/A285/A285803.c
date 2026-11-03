/*
 * A285803  Numbers k such that both k and 2k contain the digit 7.
 * https://oeis.org/A285803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285803: numbers k such that both k and 2k contain digit 7.
   Computed via a linear search with caching. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Check if the decimal representation of x contains the digit 7. */
static unsigned char contains_digit7(seq_t x)
{
    while (x)
    {
        if ((x % 10) == 7)
            return 1;
        x /= 10;
    }
    return 0;
}

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
