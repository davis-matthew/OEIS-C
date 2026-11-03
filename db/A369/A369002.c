/*
 * A369002  Numbers k for which k' / gcd(k,k') is even, where k' stands for
 *          the arithmetic derivative of k, A003415.
 * https://oeis.org/A369002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7381, big any n
 * Validated: standard, pass, 86 terms through n = 9855 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369002: numbers k for which k' / gcd(k,k') is even,  
   where k' is the arithmetic derivative (OEIS A003415).  
   The algorithm checks the 2‑adic valuation of k' by summing the
   individual terms of the derivative modulo 2^(e₂+1), where e₂ is
   the exponent of 2 in k.  A number qualifies iff this sum vanishes,
   i.e. v₂(k') > e₂, which is equivalent to the required division
   being even. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether k satisfies v₂(k') > e₂ */
static bool qualifies_uint64(uint64_t k)
{
    uint64_t t = k;
    int e2 = 0;
    while ((t & 1ULL) == 0) {
        ++e2;
        t >>= 1;
    }

    /* modulus mask 2^(e2+1) – 1 */
    unsigned __int128 mask = (((unsigned __int128)1) << (e2 + 1)) - 1;

    unsigned __int128 sum = 0;

    /* term for the factor 2 */
    if (e2 > 0) {
        unsigned __int128 term = ((unsigned __int128)k * (unsigned __int128)e2) / 2;
        sum = (sum + term) & mask;
    }

    /* factorize the remaining odd part */
    uint64_t temp = t;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            unsigned __int128 term = ((unsigned __int128)k * (unsigned __int128)e) / p;
            sum = (sum + term) & mask;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        unsigned __int128 term = ((unsigned __int128)k) / temp;   /* e = 1 */
        sum = (sum + term) & mask;
    }

    return (sum & mask) == 0;
}

/* integer types supplied by the build (seq_t, idx_t) */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* grow the cache array to at least n elements */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* grow capacity if necessary */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_cache = mem_resize(seq_t, seq_cache, new_cap);
        if (!new_cache) {
            /* allocation failure – aborting is safest */
            exit(EXIT_FAILURE);
        }
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    /* generate missing terms */
    while (cache_len < (size_t)n) {
        while (!qualifies_uint64(next_candidate))
            ++next_candidate;
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but keep the build happy */
    ensure_cache(n);
    return seq_cache[n - 1];
}
