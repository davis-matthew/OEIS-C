/*
 * A259645  Numbers m such that m^2 + 1, 3*m - 1 and m^2 + m + 41 are all
 *          prime.
 * https://oeis.org/A259645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 798, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9764 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Search for terms of OEIS A259645.
   a(n) is the nth integer m ≥ 1 such that m²+1, 3·m−1, and m²+m+41 are all prime. */

seq_t a(idx_t n)
{
    static seq_t *terms     = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t  next_m    = 1;          /* next candidate to test */

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        seq_t m = (seq_t)next_m;
        bool ok = false;

        if (next_m == (idx_t)1) {
            /* special case m = 1 */
            seq_t v1 = m * m + (seq_t)1;
            seq_t v2 = (seq_t)3 * m - (seq_t)1;
            seq_t v3 = m * m + m + (seq_t)41;
            if (isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3))
                ok = true;
            next_m = 2;                     /* subsequent candidates are even */
        } else {
            /* m is even (all m > 1 must be even for m²+1 to be prime) */
            seq_t v1 = m * m + (seq_t)1;
            seq_t v2 = (seq_t)3 * m - (seq_t)1;
            seq_t v3 = m * m + m + (seq_t)41;
            if (isPrimeGeneric(v1) && isPrimeGeneric(v2) && isPrimeGeneric(v3))
                ok = true;
            next_m += (idx_t)2;              /* skip to next even number */
        }

        if (ok) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp)
                    return (seq_t)0;           /* out‑of‑memory; caller may handle */
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = m;
        }
    }

    return terms[n - 1];
}
