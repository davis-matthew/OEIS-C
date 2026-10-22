/*
 * A385585  G.f. A(x) satisfies A(x) = Sum_{k>=0} (k*x)^k * A(k*x).
 * https://oeis.org/A385585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = Sum_{k=0}^{n-1} (n-k)^n * a(k) */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static seq_t *powers = NULL;
static size_t powers_cap = 0;
static idx_t computed_max = 0;

seq_t a(idx_t n) {
    /* Ensure storage for requested index */
    ensure_cache(n, &cache, &cache_cap);
    if (cache[0] == (seq_t)0) /* initialise a(0) */
        cache[0] = (seq_t)1;

    /* Fast return if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Ensure powers array is large enough */
    ensure_cache(n, &powers, &powers_cap);

    /* Compute missing values up to n */
    for (idx_t len = computed_max + 1; len <= n; ++len) {
        /* Update powers of existing integers to the new exponent len */
        for (idx_t i = 1; i < len; ++i) {
            powers[i] = powers[i] * (seq_t)i;
        }
        /* Compute the new power len^len */
        powers[len] = big_pow((seq_t)len, (seq_t)len);

        /* Compute a[len] = Σ_{i=1}^{len} powers[i] * a[len-i] */
        seq_t sum = 0;
        for (idx_t i = 1; i <= len; ++i) {
            sum += powers[i] * cache[len - i];
        }
        cache[len] = sum;
    }

    computed_max = n;
    return cache[n];
}
