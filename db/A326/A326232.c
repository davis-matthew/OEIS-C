/*
 * A326232  Numbers k such that N = k^2 is a twin rank (cf. A002822: 6N +- 1
 *          are twin primes).
 * https://oeis.org/A326232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by searching for integer k such that 6*k^2-1
 *            and 6*k^2+1 are both prime; a(1)=1, all other k are multiples of
 *            5
 * Range:     u8 n <= 9, u16 n <= 515, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* fast deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                            19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    /* deterministic bases for n < 2^64 */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1;
        __uint128_t power = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n-1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x64 = (__uint128_t)x64 * x64 % n;
            if (x64 == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of discovered k values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;
    static idx_t next_k      = 5;     /* next candidate after 1 */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_cap   = 64;
        cache       = (seq_t*)malloc((size_t)cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)1;        /* a(1) = 1 */
        cache_size  = 1;
        next_k      = 5;               /* first multiple of 5 */
    }

    /* extend cache until it contains at least n terms */
    while (cache_size < n) {
        seq_t k = (seq_t)next_k;
        uint64_t kk   = (uint64_t)k;
        uint64_t val1 = 6ULL * kk * kk - 1ULL;
        uint64_t val2 = 6ULL * kk * kk + 1ULL;

        if (is_prime_uint64(val1) && is_prime_uint64(val2)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache,
                                        (size_t)cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;
        }
        next_k += 5;
    }

    return cache[n-1];
}
