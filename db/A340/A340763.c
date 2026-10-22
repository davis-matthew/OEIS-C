/*
 * A340763  Number of primes p <= n that are congruent to 1 modulo 3.
 * https://oeis.org/A340763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes p ≤ n with p ≡ 1 (mod 3)
 * Range:     u8 n <= 3708, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cumulative array of the required counts.
   Whenever a larger n is queried the array is extended and all missing
   entries are computed on the fly using the provided is_prime function. */

/* The function size is O(n_max) overall, where n_max is the largest n
   ever requested.  Each number in the range (previous_max, n] is tested
   for primality once.  Queries can be made in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of cumulative counts.
       cum[i] contains a(i).  The array is 1‑based but cum[0] = 0 for
       simplicity. */
    static idx_t max_n      = 0;   /* maximum n for which cum is valid  */
    static seq_t *cum       = NULL;/* cumulative array            */

    /* Handle trivial small n */
    if (n == (idx_t)0) return 0;

    /* Extend the cache if necessary */
    if (n > max_n)
    {
        /* Allocate a new array big enough for all indices up to n.
           calloc ensures the new part is zero‑initialized. */
        seq_t *new_cum = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!new_cum) return 0;      /* allocation failure – return 0 */

        /* Copy previous data */
        if (cum)
            for (idx_t i = 0; i <= max_n; ++i)
                new_cum[i] = cum[i];

        /* Free the old array */
        free(cum);
        cum = new_cum;

        /* Compute the missing entries */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            bool prime = is_prime(i);
            seq_t inc = (prime && (i % (idx_t)3 == (idx_t)1)) ? (seq_t)1 : (seq_t)0;
            cum[i] = cum[i - 1] + inc;
        }

        max_n = n;
    }

    /* Return the cached value */
    return cum[n];
}
