/*
 * A362734  E.g.f. satisfies A(x) = exp(x + x * A(x)^3).
 * https://oeis.org/A362734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0}^{n} (3*k+1)^{\,n-1} * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, so we handle it explicitly.  The function uses the
   provided utilities for binomial coefficients and exponentiation.  */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;          /* cache of computed values          */
static size_t cache_cap = 0;         /* current capacity of 'cache'       */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Ensure the cache is large enough for index n */
    ensure_cache(n, &cache, &cache_cap);
    if (cache[n] != (seq_t)0)             /* already computed?               */
        return cache[n];

    seq_t sum = 0;   /* accumulator for the sum */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(3 * (seq_t)k + (seq_t)1);   /* 3*k+1   */
        seq_t power = big_pow(base, (seq_t)(n - 1));     /* (3k+1)^{n-1} */
        seq_t comb  = binomial(n, k);                   /* C(n,k)   */
        seq_t term  = comb * power;                     /* term product */
        sum += term;                                    /* accumulate */
    }

    cache[n] = sum;   /* store result for future calls */
    return sum;
}
