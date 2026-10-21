/*
 * A274794  Numbers n such that n^3 is the sum of two triangular numbers in
 *          exactly one way.
 * https://oeis.org/A274794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of sequence A274794: numbers n such that n³ is a sum of two triangular numbers in exactly one way. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Utility for prime factorisation ---------------------------------------------------- */
/* This header provides the function:
 *   void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt);
 * which fills the arrays with the prime factors (primes[i]) and their exponents (exps[i]),
 * and sets *cnt to the number of distinct prime factors.                                   */
#include "../utils/factor_u64.h"

/* --- Integer square‑root for 64‑bit values ---------------------------------------------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 4294967296ULL;  /* 2^32, > sqrt(2^64-1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* --- Sequence cache --------------------------------------------------------------- */
static seq_t *seq_cache   = NULL;
static size_t cache_len   = 0;        /* number of elements stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static idx_t  last_checked = 0;       /* next candidate to test */

/* --- Main sequence generator -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* values below the offset are irrelevant */
        return 0;

    /* Grow the cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        /* candidate value m */
        uint64_t m = (uint64_t)last_checked;

        /* Compute N = 8*m³ + 2 using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 cube = (unsigned __int128)m * m * m;
        unsigned __int128 N128 = cube * 8 + 2;
        uint64_t N = (uint64_t)N128;          /* fits into 64‑bit for the intended range */

        /* Factorise N */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t  cnt = 0;
        factor_u64(N, primes, exps, &cnt);

        /* Compute r₂: number of representations of N as a sum of two squares */
        uint64_t prod = 1;
        bool     odd3 = false;                /* true if a prime ≡3 (mod4) has odd exponent */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            if ((p & 3ULL) == 3ULL) {         /* p ≡ 3 (mod4) */
                if (e & 1ULL) { odd3 = true; break; }
            } else if ((p & 3ULL) == 1ULL) {  /* p ≡ 1 (mod4) */
                prod *= (e + 1ULL);
            }
            /* prime 2 is ignored – it does not affect d1 or d3 in the formula */
        }

        uint64_t r2 = 0;
        if (!odd3) r2 = 4ULL * prod;         /* r₂ formula for all representations */

        /* Check for the special equal‑pair case: N = 2 * x² */
        bool equal_pair = false;
        if ((N & 1ULL) == 0) {
            uint64_t half = N >> 1;          /* N / 2 */
            uint64_t s = isqrt_u64(half);
            if (s * s == half) equal_pair = true;
        }

        /* Determine uniqueness of representation */
        bool unique = false;
        if (equal_pair) {
            if (r2 == 4ULL) unique = true;   /* the sole representation with x = y */
        } else {
            if (r2 == 8ULL) unique = true;   /* exactly one unordered pair */
        }

        /* If unique, store m in the cache */
        if (unique) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 64U;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_len++] = (seq_t)m;
        }

        ++last_checked;
    }

    return seq_cache[(size_t)n - 1];
}
