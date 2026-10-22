/*
 * A325299  a(n) = 9 * sigma(n).
 * https://oeis.org/A325299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * sigma(n) (sigma is the sum of all divisors of n)
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma_cache = NULL;
static size_t cache_size  = 0;

/* Build a table of sigma values up to 'upto' (inclusive). */
static void build_sigma_cache(idx_t upto)
{
    if (upto <= (idx_t)cache_size)
        return;

    /* Allocate a fresh array, zero‑initialized */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[(size_t)(upto + 1)]();
#else
    seq_t *new_cache = (seq_t *)calloc((size_t)(upto + 1), sizeof(seq_t));
#endif

    /* Compute divisor sums */
    for (idx_t i = 1; i <= upto; ++i)
    {
        for (idx_t j = i; j <= upto; j += i)
        {
            new_cache[j] += (seq_t)i;
        }
    }

    /* Discard the old cache and store the new one */
#ifdef __cplusplus
    delete[] sigma_cache;
#else
    free(sigma_cache);
#endif
    sigma_cache = new_cache;
    cache_size  = (size_t)upto;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is defined for n >= 1 */
        return (seq_t)0;

    build_sigma_cache(n);
    return sigma_cache[n] * (seq_t)9;
}
