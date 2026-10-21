/*
 * A246666  Numbers n such that n^3 + (n+1)^3 + (n+3)^3 is prime.
 * https://oeis.org/A246666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 1753, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246666
   Seek the n‑th integer m such that m³ + (m+1)³ + (m+3)³ is prime.
   The sequence is stored in ascending order; a(1)=1, a(2)=3, … */

/* Build a sparse cache of found terms.  On each call the function
   extends the cache until it contains the requested index in O(1)
   amortised time.  The computation of the prime test is performed
   with the supplied big‑integer type seq_t, so the code works for
   any supported seq_t implementation. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_size = 0;    /* how many cached */
static size_t cache_cap  = 0;    /* allocator capacity */

/* Return the n‑th term of the sequence (n >= 1).  The function
   returns 0 when n == 0 or if memory allocation fails. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1 */
        return 0;

    /* Already computed?  */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Iterate odd candidates, starting after the largest cached
       term or from 1 if the cache is empty.  */
    seq_t cand = 1;
    if (cache_size > 0)
        cand = cache[cache_size - 1] + 2;     /* skip even numbers */

    /* Extending the cache until we reach the desired index.  */
    while ((size_t)n > cache_size)
    {
        /* Compute 3*n³ + 12*n² + 30*n + 28  (the given expression).  */
        seq_t n2 = cand * cand;
        seq_t n3 = n2 * cand;
        seq_t expr = (seq_t)3 * n3 + (seq_t)12 * n2 + (seq_t)30 * cand + (seq_t)28;

        if (isPrimeGeneric(expr))
        {
            /* Ensure sufficient capacity.  */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)          /* allocation failed */
                    return 0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        cand += 2;                     /* next odd candidate */
    }

    return cache[(size_t)n - 1];
}
