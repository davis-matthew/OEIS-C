/*
 * A374911  a(n) = a(2^n mod n) + a(3^n mod n), with a(0) = 1.
 * https://oeis.org/A374911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = a(2^n mod n) + a(3^n mod n) for n>0
 * Range:     u8 n <= 3219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache is large enough to hold indices up to n */
static void ensure_cache(idx_t n) {
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        /* initialise unused part with 0 (uncomputed sentinel) */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Modular exponentiation, base is small (2 or 3), modulus fits in idx_t */
static uint64_t powmod_uint64(uint64_t base, idx_t exp, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* base case */
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];   /* already computed */

    uint64_t mod = n;
    idx_t x = (idx_t)powmod_uint64(2, n, mod);
    idx_t y = (idx_t)powmod_uint64(3, n, mod);

    seq_t val = a(x) + a(y);
    cache[n] = val;
    return val;
}
