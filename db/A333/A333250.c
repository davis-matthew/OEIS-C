/*
 * A333250  a(n) = concatenate(n-1, a(n-1)) mod n, with initial condition a(0)
 *          = 0.
 * https://oeis.org/A333250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = concatenate(n-1, a(n-1)) mod n (a(0) = 0)
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are supplied by the build environment. */
static seq_t *cache = NULL;          /* cached terms */
static idx_t cache_limit = 0;        /* number of terms stored (cache_limit == size of cache) */

static void compute_to(idx_t n)
{
    if (n < cache_limit)
        return;                     /* already computed */

    /* Allocate or enlarge the cache. */
    size_t new_limit = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_limit * sizeof(seq_t));
    if (!tmp)                       /* allocation failure – nothing more we can do */
        return;
    cache = tmp;

    /* Initialise the very first term if necessary. */
    if (cache_limit == 0) {
        cache[0] = (seq_t)0;
        cache_limit = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t k = cache_limit; k <= n; ++k) {
        seq_t prev = cache[k - 1];

        /* Determine the number of decimal digits of prev. */
        int digits = 0;
        idx_t t = (idx_t)prev;
        if (t == 0)
            digits = 1;
        else {
            while (t > 0) {
                t /= 10;
                ++digits;
            }
        }

        /* Compute 10^digits. */
        idx_t pow10 = 1;
        for (int i = 0; i < digits; ++i)
            pow10 *= 10;

        /* a(k) = ((k-1) * 10^digits + prev) mod k. */
        idx_t mod = (idx_t)k;
        idx_t val = ((idx_t)(k - 1) * pow10 + (idx_t)prev) % mod;
        cache[k] = (seq_t)val;
    }

    cache_limit = (idx_t)n + 1;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[(size_t)n];
}
