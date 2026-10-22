/*
 * A352299  Expansion of e.g.f. 1/(2 - exp(x) - x^3).
 * https://oeis.org/A352299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*a(n-3) + Sum_{k=1..n} binomial(n,k)*a(n-k)
 *            Initial values: a(0) = 1, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* array holding computed a(i) values   */
static idx_t  cache_cap = 0;         /* current allocated capacity          */
static idx_t  computed = 0;          /* highest index that has been computed */

/* Ensure that cache has room up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the first three terms if needed. */
static void init_base(void)
{
    if (computed >= 2) return;
    ensure_cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    computed = 2;
}

seq_t a(idx_t n)
{
    init_base();

    if (n <= computed)
        return cache[n];

    /* Compute terms up to n (in increasing order). */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        ensure_cache(i);

        /* Sum_{k=1..i} binomial(i,k) * a(i-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += binomial(i, k) * cache[i - k];

        /* i*(i-1)*(i-2)*a(i-3) */
        seq_t prod = (seq_t)i *
                     (seq_t)(i - 1) *
                     (seq_t)(i - 2) *
                     cache[i - 3];

        cache[i] = prod + sum;
    }

    computed = n;
    return cache[n];
}
