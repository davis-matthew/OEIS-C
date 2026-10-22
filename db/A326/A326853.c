/*
 * A326853  BII-numbers of set-systems where every two covered vertices appear
 *          together in some edge (cointersecting).
 * https://oeis.org/A326853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_cointersecting
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the n‑th BII‑number whose set‑system is cointersecting.
   The sequence is defined by searching over non‑negative integers and
   keeping those whose BII‑derived set‑system satisfies the
   cointersecting property.  The function is memoised so that calls in
   arbitrary order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_cointersecting.h"

/* Cache of already found terms (memoisation).  We store the terms
   directly in an array of seq_t, so the cache works for both
   integral and arbitrary‑precision builds. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_candidate = 0;

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (is_cointersecting(next_candidate))
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (new_arr == NULL) abort();      /* out of memory */
                cache      = new_arr;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    /* Return the stored value.  Casting is trivial for integral
       seq_t types and is a no‑op for arbitrary‑precision types. */
    return cache[(size_t)n - 1];
}
