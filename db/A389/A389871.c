/*
 * A389871  G.f. A(x) satisfies A(x) = 1 + x*(1+x^2)^2*A(x)^2.
 * https://oeis.org/A389871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*(n-2*k),k) * Catalan(n-2*k)
 *            Recurrence derived from the GF: A(x)=1+x(1+x²)²A(x)² leads to
 *            a(0)=1 a(n)= Σ_{i=0}^{n-1} a(i)*a(n-1-i) // shift 1 +2
 *            Σ_{i=0}^{n-3} a(i)*a(n-3-i) (n≥3) // shift 3 + Σ_{i=0}^{n-5}
 *            a(i)*a(n-5-i) (n≥5) // shift 5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, abort */
#include <string.h>   /* memcpy */

static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_cap = 0;       /* current capacity of cache */
static idx_t cache_last = (idx_t)-1; /* last index computed */

static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    if (new_cap < (size_t)(n + 1))
        new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();
    if (cache) {
        memcpy(tmp, cache, sizeof(seq_t) * cache_cap);
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    /* initialise the cache with a(0)=1 if this is the first call */
    if (cache_last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    /* if the requested value is already computed, return it */;
    if (n <= cache_last)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* shift 1 contribution: Σ_{j=0}^{i-1} a[j] * a[i-1-j] */
        for (idx_t j = 0; j < i; ++j) {
            sum += cache[j] * cache[i - 1 - j];
        }

        /* shift 3 contribution (if i ≥ 3): 2 * Σ_{j=0}^{i-3} a[j] * a[i-3-j] */
        if (i >= 3) {
            seq_t s3 = 0;
            for (idx_t j = 0; j <= i - 3; ++j) {
                s3 += cache[j] * cache[i - 3 - j];
            }
            sum += (seq_t)2 * s3;
        }

        /* shift 5 contribution (if i ≥ 5): Σ_{j=0}^{i-5} a[j] * a[i-5-j] */
        if (i >= 5) {
            seq_t s5 = 0;
            for (idx_t j = 0; j <= i - 5; ++j) {
                s5 += cache[j] * cache[i - 5 - j];
            }
            sum += s5;
        }

        cache[i] = sum;
    }

    cache_last = n;
    return cache[n];
}
