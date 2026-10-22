/*
 * A337059  E.g.f.: 1 / (2 + x^3/6 - exp(x)).
 * https://oeis.org/A337059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1; a(n)= n*a(n-1)+n*(n-1)*a(n-2)/2 + Σ_{k=4..n}
 *            binomial(n,k)*a(n-k) (OEIS A337059)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;       /* cache for computed terms */
static size_t cap   = 0;          /* current capacity of cache */
static idx_t  computed = 0;       /* highest index computed so far */

/* ensure cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)alloc_seq_array(new_cap);
    if (!new_cache) return; /* allocation failure – let caller handle */

    if (cache) {
        for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        free_cache(cache);
    }

    cache = new_cache;
    cap   = new_cap;
    /* initialise new slots with 0 – they will be filled when computed */
    for (size_t i = 0; i < cap; ++i)
        if (i > computed) cache[i] = 0;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1) return (seq_t)1;

    /* initialise cache with first two terms */
    if (!cache) {
        ensure_capacity(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* ensure capacity for required index */
    ensure_capacity(n);
    if (n <= computed) return cache[n];

    /* compute terms sequentially up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* compute the first part:   n*a(n-1) + n*(n-1)*a(n-2)/2  */
        seq_t term1 = (seq_t)m * cache[m - 1]
                    + ((seq_t)m * (seq_t)(m - 1) * cache[m - 2]) / 2;

        /* compute the summation part */
        seq_t sum = 0;
        if (m >= 4) {
            seq_t binom = 1;          /* C(m,0) */
            for (idx_t k = 1; k <= m; ++k) {
                binom = binom * (m - k + 1) / k;  /* C(m,k) */
                if (k >= 4) {
                    sum += binom * cache[m - k];
                }
            }
        }

        cache[m] = term1 + sum;
    }

    computed = n;
    return cache[n];
}
