/*
 * A346139  Numbers k that require fewer than k steps to reach 1 under the
 *          3x+1 map.
 * https://oeis.org/A346139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of those positive integers k whose 3x+1 total
 * stopping time A006577(k) is strictly less than k.
 * The implementation proceeds by testing successive k values and
 * storing those that satisfy the inequality in a resizable array.
 * This allows calls to a(n) in arbitrary order; the cache expands
 * lazily until enough terms have been collected. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

/* Return the total stopping time of n under the Collatz map. */
static inline idx_t collatz_steps(seq_t n)
{
    idx_t   steps = 0;
    seq_t   cur   = n;

    while (cur != (seq_t)1)
    {
        if ((cur & (seq_t)1) == 0)          /* even */
            cur >>= 1;
        else                                /* odd  */
            cur = 3 * cur + (seq_t)1;

        ++steps;
    }
    return steps;
}

/* a(n) returns the nth term of A346139 (1‑indexed). */
seq_t a(idx_t n)
{
    /* Cache of sequence terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* allocated capacity */

    /* Next integer to test for inclusion. */
    static idx_t next_k = 1;

    /* Grow the term array when necessary. */
    if (terms_len == terms_cap)
    {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_terms = alloc_seq_array(new_cap);
        /* Copy existing terms to the new array. */
        for (size_t i = 0; i < terms_len; ++i)
            new_terms[i] = terms[i];
        if (terms)
            free_cache(terms);
        terms      = new_terms;
        terms_cap  = new_cap;
    }

    /* Advance the cache until it contains at least n terms. */
    while (terms_len < n)
    {
        idx_t steps = collatz_steps((seq_t)next_k);
        if (steps < next_k)                /* satisfy the inequality */
        {
            /* Ensure capacity for the new term. */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap * 2;
                seq_t *new_terms = alloc_seq_array(new_cap);
                for (size_t i = 0; i < terms_len; ++i)
                    new_terms[i] = terms[i];
                if (terms)
                    free_cache(terms);
                terms      = new_terms;
                terms_cap  = new_cap;
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
