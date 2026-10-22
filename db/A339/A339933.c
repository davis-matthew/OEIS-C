/*
 * A339933  Number of partitions of n into two parts with the same sum of
 *          divisors.
 * https://oeis.org/A339933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..⌊n/2⌋} [σ(k) = σ(n-k)]
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into two parts with equal
   divisor–sum, using a simple sieve to pre‑compute σ values. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memset */

extern seq_t a(idx_t n);

/* Cache of σ(k) for 0 … current_max.  Index 0 is unused. */
static idx_t current_max = 0;
static seq_t *sigma_cache = NULL;

seq_t a(idx_t n)
{
    /* an index of 0 is not part of the OEIS range */
    if (n == 0)
        return (seq_t)0;

    /* (Re)allocate cache if necessary.  Must cast the result of
       realloc because seq_t may be a class type. */
    if (n > current_max)
    {
        seq_t *new_cache = (seq_t *)realloc(sigma_cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;  /* allocation failure – unlikely */

        sigma_cache = new_cache;

        /* Initialise all entries to 0; this works for scalar types
           and for big‑int types because we assign a literal 0. */
        for (idx_t i = 0; i <= n; ++i)
            sigma_cache[i] = (seq_t)0;

        /* Compute σ(j) for all 1 ≤ j ≤ n using a divisor sieve. */
        for (idx_t i = 1; i <= n; ++i)
        {
            for (idx_t j = i; j <= n; j += i)
                sigma_cache[j] += (seq_t)i;
        }

        current_max = n;
    }

    /* Count pairs (k, n‑k) with equal σ-values. */
    seq_t count = (seq_t)0;
    for (idx_t k = 1; k <= n / 2; ++k)
    {
        if (sigma_cache[k] == sigma_cache[n - k])
            ++count;
    }

    return count;
}
