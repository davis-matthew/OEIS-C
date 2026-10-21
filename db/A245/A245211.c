/*
 * A245211  a(n) = Sum_{(d<n) | n} (d * tau(d)).
 * https://oeis.org/A245211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} (d * tau(d))
 * Range:     u8 n <= 47, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static caches for a(n) and τ(n) */
static seq_t *a_cache = NULL;
static seq_t *tau_cache = NULL;
static size_t cache_size = 0;

/* Rebuild caches up to at least new_size */
static void extend_cache(size_t new_size) {
    if (new_size <= cache_size)
        return;

    /* Allocate fresh arrays (free old ones) */
    free(a_cache);
    free(tau_cache);
    a_cache   = (seq_t *)calloc(new_size + 1, sizeof(seq_t));
    tau_cache = (seq_t *)calloc(new_size + 1, sizeof(seq_t));
    if (!a_cache || !tau_cache) {
        /* In a real system you would handle allocation failure
           more gracefully; here we simply abort the computation. */
        free(a_cache);
        free(tau_cache);
        a_cache   = NULL;
        tau_cache = NULL;
        cache_size = 0;
        return;
    }

    /* Compute τ(k) for 1 .. new_size */
    for (size_t i = 1; i <= new_size; ++i) {
        for (size_t j = i; j <= new_size; j += i) {
            tau_cache[j] = tau_cache[j] + (seq_t)1;
        }
    }

    /* Compute a(k) for 1 .. new_size */
    for (size_t d = 1; d <= new_size - 1; ++d) {
        seq_t contrib = (seq_t)d * tau_cache[d];
        for (size_t m = 2 * d; m <= new_size; m += d) {
            a_cache[m] = a_cache[m] + contrib;
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;

    size_t sz = (size_t)n;
    if (sz > cache_size)
        extend_cache(sz);

    return a_cache[n];
}
