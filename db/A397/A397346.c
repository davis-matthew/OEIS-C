/*
 * A397346  O.g.f. satisfies A(x) = exp( x + Sum_{n>=2} (2*n^2-1) * a(n)*x^n /
 *          (2*n^2) ).
 * https://oeis.org/A397346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, * a(n) = 2*n*a(n-1) + n*Σ_{k=1}^{n-2}
 *            ((2*(k+1)^2-1)/(k+1)) * a(k+1)*a(n-k-1) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* array of already computed a(n) */
static size_t cache_cap = 0;         /* current allocation size */
static size_t cache_len = 0;         /* number of values stored */

static void ensure_cache(size_t needed)
{
    if (cache_cap < needed)
        ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));
}

seq_t a(idx_t n)
{
    /* initialise first two terms */
    if (cache_len == 0) {
        ensure_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    if ((idx_t)cache_len > n)    /* already computed */
        return cache[n];

    /* extend cache up to n */
    ensure_cache((size_t)n + 1);

    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0 || i == 1){
            cache[i] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;                    /* Σ term */
        seq_t n_seq = (seq_t)i;           /* convert i to seq_t once */

        for (idx_t k = 1; k <= i - 2; ++k) {
            idx_t kp1 = k + 1;
            seq_t q      = cache[kp1] / kp1;                           /* a(k+1)/(k+1) */
            seq_t factor = (seq_t)(2 * kp1 * kp1 - 1);                  /* 2*(k+1)^2-1 */
            seq_t c      = q * factor;                                 /* integer coefficient */
            seq_t tmp     = n_seq * c;                                  /* n * c   */
            seq_t term    = tmp * cache[i - k - 1];                     /* * a(n-k-1) */
            sum += term;
        }

        seq_t t = (seq_t)(2 * i) * cache[i - 1];                       /* 2*i*a(i-1) */
        cache[i] = t + sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
