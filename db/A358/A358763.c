/*
 * A358763  Numbers k for which bigomega(k) == 3 (mod 4).
 * https://oeis.org/A358763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = k where bigomega(k) ≡ 3 (mod 4)  (n‑th smallest k) */
/* Implementation uses a simple incremental sieve; it keeps a growing
   list of qualifying numbers and expands it on demand. */

#include <stdlib.h>     /* malloc, realloc, abort */
#include <stddef.h>     /* size_t */
#include "../utils/bigomega.h"   /* seq_t bigomega(idx_t) */

/* Static cache of qualifying numbers (stored as idx_t). */
static idx_t *terms   = NULL;   /* array of k that satisfy the condition */
static size_t terms_size = 0;   /* number of elements currently stored */
static size_t terms_cap   = 0;   /* capacity of the array */
static idx_t  next_k      = 2;   /* next integer to test */

/* Ensure that the cache can hold at least "needed" elements. */
static void grow_terms(size_t needed)
{
    if (terms_cap >= needed)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;   /* double until big enough */

    idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
    if (!tmp) abort(); /* out of memory */

    terms = tmp;
    terms_cap = new_cap;
}

/* Return the n-th term of A358763 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined position; return zero for safety */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (terms_size < (size_t)n) {
        grow_terms(terms_size + 1);

        /* bigomega returns seq_t; use its modulo operation. */
        if ((bigomega(next_k) % 4) == 3)
            terms[terms_size++] = next_k;

        ++next_k;
    }

    return (seq_t)terms[n - 1];
}
