/*
 * A369249  Primes of the form 4*m+3 for which there is no representation as a
 *          sum (p*q + p*r + q*r) with three odd primes p <= q <= r.
 * https://oeis.org/A369249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 924, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 78 terms through n = 8553 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369249 – “Primes of the form 4*m+3 for which there is no
 * representation as p = a*b + a*c + b*c with odd primes
 * a ≤ b ≤ c”.  The function computes the sequence by brute‑force
 * scanning primes and testing representations.  Results are cached
 * so calls may arrive in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* global data */
static uint64_t *g_primes = NULL;
static size_t     g_primes_cnt = 0;
static const idx_t LIMIT = 20000000ULL;   /* enough for n ≤ 10 000 */

/* ------------------------------------------------------------------ */
/* Test whether a 4*m+3 prime p can be represented as
 *      p = a*b + a*c + b*c            (a,b,c odd primes, a ≤ b ≤ c)
 */
static bool is_representable(uint64_t p)
{
    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint64_t a = g_primes[i];
        if (a < 3) continue;          /* skip 2 */
        if (a * a > p) break;         /* a > sqrt(p) */

        for (size_t j = i; j < g_primes_cnt; ++j) {
            uint64_t b = g_primes[j];
            if (b * b > p) break;     /* b > sqrt(p) */

            uint64_t ab = a * b;
            uint64_t s_min = 2 * ab + b * b;
            if (s_min > p) break;     /* increasing b only increases s_min */

            if (ab >= p) continue;    /* numerator non‑positive */

            uint64_t denom = a + b;
            uint64_t numer = p - ab;
            if (numer % denom != 0) continue;

            uint64_t c = numer / denom;
            if (c < b) continue;     /* enforce a ≤ b ≤ c */

            if (is_prime((idx_t)c))
                return true;          /* found a representation */
        }
    }
    return false; /* no representation */
}

/* ------------------------------------------------------------------ */
/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t prime_pos = 0;

    /* Initialise once */
    if (cache == NULL) {
        cache_cap = 256;
        cache = mem_new(seq_t, cache_cap); /* cast void* to seq_t* */
        build_primes_upto(LIMIT, &g_primes, &g_primes_cnt);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (prime_pos >= g_primes_cnt) {
            /* Should never happen with LIMIT = 20 000 000 for n ≤ 10 000 */
            return (seq_t)0;
        }

        uint64_t p = g_primes[prime_pos++];
        if (p % 4 != 3) continue;      /* only primes ≡ 3 (mod 4) */

        if (!is_representable(p)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
