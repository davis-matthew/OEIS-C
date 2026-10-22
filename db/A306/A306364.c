/*
 * A306364  Triangular array of the number of binary, rooted, leaf-labeled
 *          tree topologies with n leaves and k cherries, n >= 2, 1 <= k <=
 *          floor(n/2).
 * https://oeis.org/A306364
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = n! (n-2)! / (2^(2k-1) (n-2k)! k! (k-1)!) (Triangular
 *            array indexing)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Cached factorials up to the largest value seen so far. */
static seq_t *fact_cache = NULL;
static idx_t fact_max = 0;

/* Compute factorial with incremental caching. */
static seq_t factorial_cached(idx_t n)
{
    if (n <= fact_max)
        return fact_cache[n];

    /* Allocate or expand cache. */
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!fact_cache) return (seq_t)0;
        fact_cache[0] = (seq_t)1;
        fact_cache[1] = (seq_t)1;
        fact_max = 1;
    } else {
        fact_cache = (seq_t *)realloc(fact_cache, sizeof(seq_t) * (n + 1));
        if (!fact_cache) return (seq_t)0;
    }

    for (idx_t i = fact_max + 1; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    fact_max = n;
    return fact_cache[n];
}

seq_t a(idx_t n)
{
    /* The sequence starts at index 2. For n < 2 the behaviour is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* Convert OEIS index n to a zero‑based term position p. */
    idx_t p = n - 2;                 /* 0‑based position in the flattened array */

    /* Find the row r (number of leaves) containing this term. */
    idx_t r = 2;                      /* minimal leaf count */
    idx_t cum = 0;                    /* terms before current row */

    /* cum + floor(r/2) gives the cumulative terms up to and including row r. */
    while (cum + (r / 2) <= p) {
        cum += r / 2;
        r++;
    }

    /* Position within the row.  Offset is 0‑based; k is 1‑based. */
    idx_t offset = p - cum;
    idx_t k = offset + 1;

    /* Compute the formula T(r,k).  All arithmetic is done in seq_t. */
    seq_t num = factorial_cached(r);
    num = num * factorial_cached(r - 2);

    seq_t denom = factorial_cached(r - 2 * k);
    denom = denom * factorial_cached(k);
    denom = denom * factorial_cached(k - 1);
    denom = denom * pow2(2 * k - 1);

    return num / denom;
}
