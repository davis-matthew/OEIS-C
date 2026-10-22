/*
 * A385530  E.g.f. A(x) satisfies A(x) = exp(x*A(-4*x)).
 * https://oeis.org/A385530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   binomial, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1
   a(n)=∑_{k=0}^{n-1} (k+1)·(-4)^k·binomial(n-1,k)·a(k)·a(n-1-k)  for n>0
*/
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static seq_t *pow_neg4 = NULL;
static size_t pow_cap   = 0;

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (cache_len == 0)
    {
        ensure_capacity(1, &cache_cap, (void**)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;

        ensure_capacity(1, &pow_cap, (void**)&pow_neg4, sizeof(seq_t));
        pow_neg4[0] = (seq_t)1;
        pow_cap = 1;
    }

    /* extend caches to hold all indices up to n */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
    if ((size_t)n + 1 > pow_cap)
        ensure_capacity((size_t)n + 1, &pow_cap, (void**)&pow_neg4, sizeof(seq_t));

    /* compute terms up to n (if not already computed) */
    for (size_t cur = cache_len; cur <= (size_t)n; ++cur)
    {
        seq_t sum = 0;
        for (size_t k = 0; k < cur; ++k)
        {
            /* term = (k+1) * pow_neg4[k] * binomial(cur-1,k) * cache[k] * cache[cur-1-k] */
            seq_t term = (seq_t)(k + 1);
            term *= pow_neg4[k];
            term *= binomial((idx_t)cur - 1, (idx_t)k);
            term *= cache[k];
            term *= cache[cur - 1 - k];
            sum += term;
        }
        cache[cur] = sum;
        pow_neg4[cur] = pow_neg4[cur - 1] * (seq_t)(-4);
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
