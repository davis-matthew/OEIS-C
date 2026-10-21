/*
 * A267603  Primes that are squares (mod 95).
 * https://oeis.org/A267603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_sq_mod5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p is a quadratic residue modulo 95. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_sq_mod5.h"

/* Quadratic residues modulo 19 (precomputed). */
static const bool SQ_MOD19[19] = {
    /* 0*/ true,  /* 1*/ true,  /* 2*/ false, /* 3*/ false,
    /* 4*/ true,  /* 5*/ true,  /* 6*/ true,  /* 7*/ true,
    /* 8*/ false, /* 9*/ true,  /*10*/ false, /*11*/ true,
    /*12*/ false,/*13*/ false,/*14*/ false,/*15*/ false,
    /*16*/ true, /*17*/ true, /*18*/ false
};

static inline bool is_sq_mod19(uint64_t x)
{
    return SQ_MOD19[x % 19];
}

/* Very simple deterministic primality test for 64‑bit integers.
   It is adequate for primes up to a few million (used here for n≈10⁴). */
static bool is_prime_simple(uint64_t n)
{
    if (n < 2u) return false;
    if (n % 2u == 0u) return n == 2u;
    for (uint64_t d = 3u; d * d <= n; d += 2u)
        if (n % d == 0u) return false;
    return true;
}

/* --- Cache of found sequence terms --- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
/* Next candidate prime (always odd, ≥3). */
static uint64_t next_candidate = 3u;

/* Ensure that at least n terms are stored in cache. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* Grow cache storage if needed. */
    if (cache_len == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n)
    {
        /* Find the next prime candidate. */
        uint64_t cand = next_candidate;
        next_candidate = cand + 2u; /* next odd number */

        if (!is_prime_simple(cand))
            continue; /* skip non‑prime */

        /* Check the quadratic‑residue condition modulo 95. */
        if (is_sq_mod5(cand) && is_sq_mod19(cand))
        {
            /* Store the valid prime in the cache. */
            if (cache_len >= cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* The function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;        /* offset is 1, so n==0 is undefined */
    ensure_cache(n);
    return cache[(size_t)(n - 1u)];
}
