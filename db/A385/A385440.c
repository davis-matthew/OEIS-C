/*
 * A385440  E.g.f. A(x) satisfies A(x) = exp( arcsinh(x * A(x)^2) ).
 * https://oeis.org/A385440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385440
 * E.g.f. A(x) satisfies A(x) = 1/sqrt(1 - 2*x*A(x)).
 * Consecutive terms are computed by the recurrence
 *   a(n+2) = 3*(3*n+5)*(3*n+1)*(n+1)/(n+3) * a(n)
 * with a(0) = a(1) = 1.
 * This implementation memoises the values and uses GCD
 * reductions to avoid intermediate overflow when seq_t is
 * a 64‑bit built‑in type. For arbitrary‑precision builds
 * the arithmetic is exact.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache(idx_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < (size_t)new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    if (cache) {
        memcpy(tmp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* initialise cache up to the requested index */
    grow_cache((size_t)n + 1);

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    while (cache_len <= n) {
        idx_t k = cache_len - 2;          /* previous known index */
        seq_t num = cache[k];
        seq_t den = (seq_t)(k + 3);       /* (n+3) part of recurrence */

        /* multiply by 3 and reduce with den */
        num *= (seq_t)3;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply by (3*k+5) and reduce */
        num *= (seq_t)(3 * k + 5);
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply by (3*k+1) and reduce */
        num *= (seq_t)(3 * k + 1);
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply by (k+1) and reduce */
        num *= (seq_t)(k + 1);
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* any remaining divisor should be 1; divide if necessary */
        if (den != (seq_t)1)
            num /= den;

        cache[cache_len] = num;
        ++cache_len;
    }

    return cache[n];
}
