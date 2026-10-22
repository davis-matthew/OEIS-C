/*
 * A316388  a(n) = a(phi(n)) + a(n-phi(n)) with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A316388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(phi(n)) + a(n - phi(n)) with a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a memoised DP table and a small totient calculator. */

#include <stdlib.h>
#include <stdint.h>

/* Periodic totient calculator for small integers. */
static inline idx_t _phi(idx_t n)
{
    idx_t result = n;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result -= result / p;
        }
    }
    if (t > 1) result -= result / t;
    return result;
}

/* Public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases explicitly. */
    if (n < 4) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* maximum index that has been cached */

    /* Allocate the cache on first use. */
    if (!cache) {
        cache_max = 3;
        cache = (seq_t *)malloc((cache_max + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* OOM protection */
        cache[0] = (seq_t)0;                  /* unused */
        cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Extend the cache if necessary. */
    if (n > cache_max) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* OOM protection */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            idx_t phi_i = _phi(i);
            idx_t rest   = i - phi_i;
            cache[i] = cache[phi_i] + cache[rest];
        }
        cache_max = n;
    }

    return cache[n];
}
