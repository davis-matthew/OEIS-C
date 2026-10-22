/*
 * A367567  a(n) = Product_{k=0..n} (3*k)! / k!^3.
 * https://oeis.org/A367567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} binomial(3*k,k) * binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* memoized values */
static size_t cache_capacity = 0;    /* allocated size of cache */
static idx_t last_computed = (idx_t)-1; /* index of last computed value */

/* Ensure cache has room for indices 0..n inclusive */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_capacity)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    assert(tmp != NULL);

    /* copy already computed values */
    for (idx_t i = 0; i <= last_computed && i < cache_capacity; ++i)
        tmp[i] = cache[i];

    free(cache);
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* initialise the base case if necessary */
    if (last_computed == (idx_t)-1) {
        assert(n >= 0);
        cache[0] = 1;
        last_computed = 0;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t bin1 = binomial(3 * i, i);
        seq_t bin2 = binomial(2 * i, i);
        seq_t prod = bin1;
        prod *= bin2;                 /* prod = bin1 * bin2 */
        cache[i] = cache[i - 1] * prod;
    }

    last_computed = n;
    return cache[n];
}
