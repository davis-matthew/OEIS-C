/*
 * A294929  Number of proper divisors of n that are abundant (A005101).
 * https://oeis.org/A294929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors of n that are abundant. An
 *            integer d>1 is abundant when the sum of its positive divisors
 *            Σ(d) exceeds 2*d; the proper sum Σ(d)–d is thus > d. a(n) counts
 *            those d|n with d<n and Σ(d)>2*d
 * Range:     u8 n <= 794, u16 n <= 9251, u32 n <= 9251, u64 n <= 9251,
 *            u128 n <= 9251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* caches: 1 … current maximum n for which a, σ and abundant has been computed */
    static seq_t *a_cache          = NULL; /* a(i) for i up to cache_size */
    static seq_t *sigma_cache      = NULL; /* sum of divisors σ(i) for i up to cache_size */
    static bool *abundant_cache    = NULL; /* true if σ(i) > 2*i for i up to cache_size */
    static idx_t cache_size        = 0;     /* last n for which everything is known */

    if (n < 1)                 /* the sequence is only defined for n ≥ 1; */
        return 0;              /* we return 0 for safety without an error. */

    if (n > cache_size)        /* extend all caches to at least n */
    {
        idx_t new_size = n;

        /* free old memory, if any */
        if (a_cache)        free(a_cache);
        if (sigma_cache)    free(sigma_cache);
        if (abundant_cache) free(abundant_cache);

        /* allocate zero‑initialized arrays */
        a_cache          = (seq_t*)calloc((size_t)(new_size + 1), sizeof(seq_t));
        sigma_cache      = (seq_t*)calloc((size_t)(new_size + 1), sizeof(seq_t));
        abundant_cache   = (bool*)calloc((size_t)(new_size + 1), sizeof(bool));

        /* compute σ(i) = sum of all positive divisors of i */
        for (idx_t i = 1; i <= new_size; ++i)
        {
            for (idx_t j = i; j <= new_size; j += i)
                sigma_cache[j] += (seq_t)i;
        }

        /* flag abundant numbers: σ(i) > 2*i  (proper sum > i) */
        for (idx_t i = 1; i <= new_size; ++i)
            abundant_cache[i] = (sigma_cache[i] > (seq_t)2 * (seq_t)i);

        /* compute a(i): number of abundant proper divisors of i */
        for (idx_t d = 1; d <= new_size; ++d)
        {
            if (!abundant_cache[d]) continue;
            for (idx_t m = d << 1; m <= new_size; m += d) /* m = 2*d,3*d … */
                a_cache[m] += (seq_t)1;
        }

        cache_size = new_size;
    }

    return a_cache[n];
}
