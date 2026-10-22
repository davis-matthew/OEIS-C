/*
 * A352800  Numbers k such that 2*k^2 + 29 is prime.
 * https://oeis.org/A352800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k ≥ 0 such that 2*k² + 29 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small deterministic Miller–Rabin for 64‑bit integers. */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t res = (__uint128_t)a * b;
    return (uint64_t)(res % mod);
}

static uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d)
    {
        if (d & 1) result = mul_mod64(result, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,
        37ULL,41ULL,43ULL,47ULL
    };
    for (size_t i=0; i<sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n-1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i=0; i<sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        int passed = 0;
        for (int r=1; r<s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { passed = 1; break; }
        }
        if (!passed) return 0;
    }
    return 1;
}

/* Memoisation state. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 0;   /* next k to test */

/* Ensure space for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new valid k to the cache. */
static void add_k(idx_t k)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = (seq_t)k;
}

/* Return the n-th element (1‑based). */
seq_t a(idx_t n)
{
    /* Build until we have at least n entries. */
    while (cache_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t val = 2ULL * k * k + 29ULL;
        if (is_prime_uint64(val)) add_k(next_k);
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
