/*
 * A359381  Nonmultiples of 3 that have an odd number of prime factors (with
 *          multiplicity).
 * https://oeis.org/A359381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th non‑multiple of 3 that has an odd number of prime
   factors (multiplicity taken into account).  The sequence is the
   OEIS A359381. */

#include <stdlib.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* next natural number to test */
    static idx_t next = 2;       /* start from the first candidate 2 */

    /* return zero for an undefined index (offset is 1) */
    if (n == 0)
        return (seq_t)0;

    /* generate terms until we have at least n entries */
    while (cache_len < n)
    {
        /* skip multiples of 3 */
        if ((next % 3) != 0)
        {
            /* Omega(next) = total number of prime factors (with multiplicity) */
            seq_t omega = count_prime_factors((seq_t)next);

            if ((omega & 1U) != 0)          /* odd number of prime factors */
            {
                /* grow the cache if needed */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        return (seq_t)0;   /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next;
            }
        }
        ++next;
    }

    return cache[n - 1];
}
