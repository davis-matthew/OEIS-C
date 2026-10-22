/*
 * A350856  Initial members of prime triples (p, p+2, p+14).
 * https://oeis.org/A350856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A350856: primes p such that p, p+2 and p+14 are all prime.
   The function searches for such primes and caches the results so
   that any call to a(n) is fast after the first computing. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility: 64‑bit prime test (deterministic Miller‑Rabin). */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof witnesses / sizeof witnesses[0]; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        unsigned __int128 x = 1, base = a, exp = d, mod = n;
        while (exp) {
            if (exp & 1) x = (x * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            xx = (uint64_t)x;
            if (xx == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_prime_candidate = 3; /* next odd number to test */

/* Ensure the cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need) {
    if (need <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 32;
    while (cache_cap < need) cache_cap = cache_cap + (cache_cap >> 1) + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n) {
    if (!cache) { /* initialise cache once */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime_candidate;
        next_prime_candidate += 2; /* next odd candidate */
        if (is_prime_u64(p) &&
            is_prime_u64(p + 2ULL) &&
            is_prime_u64(p + 14ULL)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
        /* continue searching */
    }
    return cache[n - 1];
}
