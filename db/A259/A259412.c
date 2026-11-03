/*
 * A259412  Primes of the form 1 - sigma(n) + sigma(n)^2 - sigma(n)^3 +
 *          sigma(n)^4.
 * https://oeis.org/A259412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = primes of the form 1 - σ(n) + σ(n)^2 - σ(n)^3 + σ(n)^4
 *            Returned in the order of increasing n (A259410). Every call
 *            produces the next prime in this list, skipping any n for which
 *            the expression is not prime
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 1017,
 *            u128 n <= 1017, big any n
 * Validated: standard, pass, 41 terms through n = 948 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Static cache state shared by all calls */
static seq_t *prime_cache = NULL;   /* array of found primes          */
static size_t prime_cache_len = 0;  /* number of primes stored        */
static size_t prime_cache_cap = 0;  /* allocated capacity              */
static idx_t next_tested_n     = 1; /* next n to evaluate             */

/* Return the n‑th prime of the form 1 - σ(n) + σ(n)^2 - σ(n)^3 + σ(n)^4 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* Generate primes until we have at least n of them */
    while (prime_cache_len < (size_t)n) {
        seq_t s   = sigma_of(next_tested_n);        /* σ(n)   */
        seq_t s2  = s * s;                          /* σ^2    */
        seq_t s3  = s2 * s;                         /* σ^3    */
        seq_t s4  = s3 * s;                         /* σ^4    */
        /* Compute 1 - σ + σ^2 - σ^3 + σ^4 */
        seq_t val = s4 - s3;                        /* σ^4 - σ^3   */
        val += s2;                                 /* + σ^2       */
        val -= s;                                  /* - σ         */
        val += 1;                                  /* + 1         */

        if (isPrimeGeneric(val)) {
            /* Append prime to cache */
            append_candidate(&prime_cache, &prime_cache_len,
                             &prime_cache_cap, val);
        }
        ++next_tested_n;
    }

    /* Return the requested element (1‑based index) */
    return prime_cache[n-1];
}
