/*
 * A256785  Numbers n such that digitsum(n) is a whole number when n is
 *          represented in the fractional base 1.5 = 3/2.
 * https://oeis.org/A256785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   even_H_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A256785 – nth integer whose base‑1.5 representation has an even
    number of 0.5 digits (“H” digits).  The code builds the sequence
    on demand by testing successive integers and storing the good
    ones in a static cache.  It works for any seq_t implementation
    that supports + – * / % << >> and comparison.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/even_H_digits.h"

/* ------------------------------------------------------------------ */
/* Test whether an integer m has an even number of H (0.5) digits     */
/* in its 1.5 (3/2) base representation.                              */

/* ------------------------------------------------------------------ */
/*  Cache of already computed terms.  The cache grows as needed.     */
/* ------------------------------------------------------------------ */
static seq_t *cache    = NULL; /* dynamic array of sequence values      */
static size_t cache_sz = 0;    /* number of values stored in cache      */
static size_t cache_cap = 0;   /* current capacity of cache             */
static seq_t  next_val  = 1;   /* next integer to test for good/poor   */

/* Append a value to the cache, resizing if necessary. */
static void cache_append(seq_t v)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails, the program will terminate.  The
           challenge environment does not require a graceful fail. */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = v;
}

/* ------------------------------------------------------------------ */
/*  The required public function.                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0; safe return */

    /* Expand cache until it contains at least n terms. */
    while (cache_sz < (size_t)n) {
        if (even_H_digits(next_val))
            cache_append(next_val);
        next_val++;                    /* move to next integer to test */
    }

    /* Sequence is 1‑based: a(1) is the first term stored. */
    return cache[(size_t)n - 1];
}
