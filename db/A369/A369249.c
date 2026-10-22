/*
 * A369249  Primes of the form 4*m+3 for which there is no representation as a
 *          sum (p*q + p*r + q*r) with three odd primes p <= q <= r.
 * https://oeis.org/A369249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 924, u32 n <= 8553, u64 n <= 8553,
 *            u128 n <= 8553, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime
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

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

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
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t)); /* cast void* to seq_t* */
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
