/*
 * A346188  a(1) = 1; a(n+1) = Sum_{d|n} binomial(n,d) * a(n/d).
 * https://oeis.org/A346188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; a(n+1) = Σ_{d|n} binomial(n,d) * a(n/d)
 *            (OEIS A346188)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: integer square root (floor of sqrt) */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Helper: multiplicative binomial coefficient using seq_t arithmetic */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / i;
    }
    return res;
}

/* Cache for computed terms */
static seq_t *cache = NULL;       /* cache[i] holds a(i) for i>=1 */
static size_t cache_size = 0;     /* allocated capacity (>=1) */
static idx_t cache_max = 0;       /* highest index that has been computed */

/* Ensure cache entries up to index n are computed */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;

    size_t new_size = (size_t)(n + 1);
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t m = i - 1;                 /* divisors of m */
        idx_t root = int_sqrt(m);
        for (idx_t d = 1; d <= root; ++d) {
            if (m % d != 0) continue;
            idx_t other = m / d;

            /* term for divisor d */
            seq_t bin = binomial_multiplicative(m, d);
            sum += bin * cache[other];

            if (other != d) {
                seq_t bin2 = binomial_multiplicative(m, other);
                sum += bin2 * cache[d];
            }
        }
        cache[i] = sum;
    }

    cache_max = n;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* safety, should not happen */
    ensure_cache(n);
    return cache[n];
}
