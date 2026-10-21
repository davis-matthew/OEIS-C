/*
 * A241265  Numbers n such that n^4+(n+1)^4 is not prime.
 * https://oeis.org/A241265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m such that m^4 + (m+1)^4 is composite
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* 64‑bit deterministic Miller‑Rabin.  Works for all n < 2^64. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* modular exponentiation using __int128 for multiplication */
        uint64_t x = 1;
        uint64_t pow = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (unsigned __int128)x * pow % n;
            pow = (unsigned __int128)pow * pow % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset undefined, return 0 as sentinel */

    static seq_t *cache = NULL;     /* stored qualifying n's */
    static size_t cache_len = 0;    /* number of stored terms      */
    static size_t cache_cap = 0;    /* current capacity            */
    static idx_t test_n = 1;        /* next integer to test        */

    /* return if already computed */
    if (n <= cache_len) return cache[n-1];

    /* initialise cache if needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        /* compute expr = test_n^4 + (test_n+1)^4 using 128‑bit to avoid overflow */
        unsigned __int128 a = (unsigned __int128)test_n;
        unsigned __int128 a2 = a * a;
        unsigned __int128 a4 = a2 * a2;
        unsigned __int128 b = a + 1;
        unsigned __int128 b2 = b * b;
        unsigned __int128 b4 = b2 * b2;
        unsigned __int128 expr128 = a4 + b4;

        bool composite;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            uint64_t expr64 = (uint64_t)expr128;
            composite = !is_prime_64(expr64);
        } else {
            seq_t expr = (seq_t)expr128;
            composite = !is_prime_big(expr);
        }

        if (composite) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)test_n;
        }
        ++test_n;
    }
    return cache[n-1];
}
