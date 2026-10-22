/*
 * A367785  Expansion of e.g.f. exp(exp(3*x) - x - 1).
 * https://oeis.org/A367785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = -a(n-1) + Sum_{k=1..n} binomial(n-1,k-1) * 3^k *
 *            a(n-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;               /* already computed */

    size_t new_len = (size_t)n + 1U;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    /* In a real framework we would handle allocation failure, but
       the constraints guarantee enough memory. */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = 1;       /* C(i-1,0) */
        seq_t pow3  = 3;       /* 3^1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * pow3 * a(i-k) */
            seq_t term = cache[i - k];
            term *= pow3;
            term *= binom;
            sum += term;

            if (k < i) {
                /* Update binomial coefficient: C(i-1,k) = C(i-1,k-1)*(i-k)/k */
                binom *= (seq_t)(i - k);
                binom /= (seq_t)k;
                pow3 *= 3;
            }
        }

        /* a(i) = sum - a(i-1) */
        cache[i] = sum;
        cache[i] -= cache[i - 1];
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
