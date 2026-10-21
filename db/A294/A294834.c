/*
 * A294834  Numerators of the partial sums of the reciprocals of the positive
 *          tetradecagonal numbers (k + 1)*(6*k + 1) = A051866(k+1).
 * https://oeis.org/A294834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of \sum_{k=0}^{n} 1/((k+1)*(6*k+1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised: earlier values are stored so that any call
   order is handled efficiently.  It relies only on the supplied gcd
   routine and consumes no floating point. */

#include <stdlib.h>
#include "../utils/gcd.h"

/* cached terms: numerators and denominators of V(6,1;k) for k = 0 .. n */
static seq_t *num_cache = NULL;      /* numerators   */
static seq_t *den_cache = NULL;      /* denominators */
static idx_t  cache_len  = 0;        /* number of entries currently stored   */
static idx_t  cache_cap  = 0;        /* allocated capacity (in entries)      */

/* Make sure the caches can hold at least `need` entries.  Uses malloc
   / realloc; casts are required for C++.  Since seq_t is trivially
   constructible in all supported builds, this is safe. */
static void grow_cache(idx_t need)
{
    if (cache_cap > need)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= need)
        new_cap *= 2;

    num_cache = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
    den_cache = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n-th numerator of the partial sum. */
seq_t a(idx_t n)
{
    grow_cache(n + 1);                 /* need indices 0 … n inclusive */

    /* initialise the table if this is the first call */
    if (cache_len == 0) {
        num_cache[0] = 1;
        den_cache[0] = 1;
        cache_len = 1;
    }

    /* compute successive terms until we reach n */
    while (cache_len <= n) {
        idx_t k = cache_len;                /* new index to compute */

        seq_t prev_num = num_cache[cache_len - 1];
        seq_t prev_den = den_cache[cache_len - 1];

        /* term denominator: (k+1)*(6*k+1) */
        seq_t term_den = ((seq_t)(k + 1)) * ((seq_t)(6 * k + 1));

        /* add 1/term_den to prev_num/prev_den */
        seq_t new_num = prev_num * term_den + prev_den;
        seq_t new_den = prev_den * term_den;

        /* reduce */
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num_cache[cache_len] = new_num;
        den_cache[cache_len] = new_den;
        cache_len++;
    }

    return num_cache[n];
}
