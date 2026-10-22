/*
 * A335097  Number of integers less than n with the same number of prime
 *          factors (counted with multiplicity) as n.
 * https://oeis.org/A335097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ j < n : bigomega(j) = bigomega(n) }|
 * Range:     u8 n <= 850, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"

/* Return a(n) for the OEIS sequence A335097. */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n <= 1)
        return (seq_t)0;

    /* Static cache of BigOmega values up to the maximum requested n. */
    static seq_t *bigomega_cache = NULL;
    static idx_t cache_size   = 0;

    /* Expand cache if necessary. */
    if (cache_size < n)
    {
        /* Allocate new cache (index 0..n). */
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;                 /* allocation failure */

        /* Build smallest prime factor array up to n. */
        idx_t *spf = NULL;
        build_spf(n, &spf);                  /* spf[0]..spf[n] */

        /* Compute BigOmega for all numbers 0..n. */
        new_cache[0] = new_cache[1] = (seq_t)0;
        for (idx_t i = 2; i <= n; ++i)
        {
            idx_t divisor = spf[i];
            idx_t reduced = i / divisor;                 /* i / spf[i] */
            new_cache[i] = new_cache[reduced] + (seq_t)1;
        }

        free(spf);                   /* free the temporary spf array */

        /* Replace old cache. */
        free(bigomega_cache);
        bigomega_cache = new_cache;
        cache_size     = n;
    }

    /* Espresso the target BigOmega value. */
    seq_t target = bigomega_cache[n];
    seq_t count  = (seq_t)0;

    /* Count all j < n with matching BigOmega. */
    for (idx_t j = 1; j < n; ++j)
    {
        if (bigomega_cache[j] == target)
            ++count;
    }

    return count;
}
