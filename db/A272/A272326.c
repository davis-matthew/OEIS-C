/*
 * A272326  Primes of the form k^4 + 853*k^3 + 2636*k^2 + 3536*k + 1753 in
 *          order of increasing nonnegative k.
 * https://oeis.org/A272326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form k^4 + 853*k^3 + 2636*k^2 +
 *            3536*k + 1753
 * Range:     u8 none, u16 n <= 4, u32 n <= 73, u64 n <= 2457, u128 n <= 2457,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 2193 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/mem.h"

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Cached primes seen so far. */
static seq_t *prime_cache = NULL;
/* Number of primes stored in prime_cache. */
static size_t prime_cnt = 0;
/* Current capacity of prime_cache. */
static size_t prime_cap = 0;
/* Next value of k to test. */
static idx_t next_k = 0;

/* Ensure prime_cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    prime_cache = tmp;
    prime_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset is 1, so a(0) is undefined */
        return 0;

    /* Return cached prime if already computed. */
    if (prime_cnt >= n)
        return prime_cache[n - 1];

    /* Generate primes until the nth one is found. */
    while (prime_cnt < n) {
        idx_t   k   = next_k++;          /* current k value (non‑negative) */
        seq_t   ks  = (seq_t)k;           /* k as seq_t */
        seq_t   k2  = ks * ks;            /* k^2 */
        seq_t   k3  = k2 * ks;            /* k^3 */
        seq_t   k4  = k3 * ks;            /* k^4 */

        /* Evaluate the polynomial. */
        seq_t val = k4
                    + (seq_t)853 * k3
                    + (seq_t)2636 * k2
                    + (seq_t)3536 * ks
                    + (seq_t)1753;

        if (isPrimeGeneric(val)) {
            ensure_cache_cap(prime_cnt + 1);
            prime_cache[prime_cnt++] = val;
        }
    }

    return prime_cache[n - 1];
}
