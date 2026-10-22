/*
 * A324356  Total number of occurrences of 6 in the (signed) displacement sets
 *          of all permutations of [n+6] divided by 6!.
 * https://oeis.org/A324356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{k+1} * (n! / k!) * C(n - k + 6, 6)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   comb6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Same as –1/720 * Σ_{j=1..n} (-1)^j * binom(n,j) * (n+6-j)! )      */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/comb6.h"

/* Cached factorials */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_max = 0;

/* Ensure factorials up to n are computed */
static void ensure_factorial_cache(idx_t n)
{
    if (fact_cache != NULL && n <= fact_cache_max)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return;                /* allocation failed – return 0 values */

    if (fact_cache != NULL) {
        memcpy(new_cache, fact_cache,
               (size_t)(fact_cache_max + 1) * sizeof(seq_t));
        free(fact_cache);
    }

    if (fact_cache == NULL) {   /* initialise from scratch */
        new_cache[0] = 1;
        seq_t val = 1;
        for (idx_t i = 1; i <= n; ++i) {
            val = val * i;
            new_cache[i] = val;
        }
    } else {                    /* extend existing cache */
        seq_t val = new_cache[fact_cache_max];
        for (idx_t i = fact_cache_max + 1; i <= n; ++i) {
            val = val * i;
            new_cache[i] = val;
        }
    }

    fact_cache = new_cache;
    fact_cache_max = n;
}

/* OEIS sequence A324356 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_factorial_cache(n);
    seq_t fact_n = fact_cache[n];

    seq_t pos = 0, neg = 0;
    seq_t prod = fact_n;           /* prod = n! / k! for current k */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m   = n - k + 6;
        seq_t bin = comb6(m);
        seq_t term = prod * bin;

        if (k & 1)
            pos += term;
        else
            neg += term;

        prod /= (k + 1);          /* update prod for next k */
    }

    seq_t result = pos;
    result -= neg;                 /* pos >= neg for these n */
    return result;
}
