/*
 * A333857  Positive odd numbers b with an unequal number of odd and even
 *          elements of the restricted residue system of the mod* congruence
 *          of Brändli and Beyne (numbers b ordered increasingly).
 * https://oeis.org/A333857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   factorize, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches successive odd numbers and selects those
 * whose distinct prime divisors are all congruent to 3 modulo 4 and at
 * least two such primes, or the value 1. Those are the unbalanced odd
 * numbers of sequence A333857. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factorize.h"   /* factorize(uint64_t n, uint64_t primes[], int exps[]) */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* ---------- helper: test if a value is unbalanced ---------- */
static bool is_unbalanced(seq_t b)
{
    /* 1 is always unbalanced */
    if (b == (seq_t)1) return true;

    /* Only odd numbers are considered */
    if ((b & (seq_t)1) == 0) return false;

    /* Factorise into distinct primes */
    uint64_t primes[64];
    int    exps[64];
    size_t k = factorize((uint64_t)b, primes, exps); /* number of distinct primes */

    size_t cnt3mod4 = 0;
    for (size_t i = 0; i < k; ++i) {
        if ((primes[i] & 0x3) == 0x1)     /* prime ≡ 1 (mod 4) => balanced */
            return false;
        /* primes[i] is odd, hence ≡ 3 (mod 4) */
        ++cnt3mod4;
    }
    return cnt3mod4 >= 2;  /* need at least two distinct 3‑mod‑4 primes */
}

/* ---------- memoisation cache ---------- */
static seq_t *cache  = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Append a found value to the cache, enlarging it if necessary. */
static void append_candidate(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
        if (!new_arr) abort();           /* out of memory */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- main function implementation ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for an out‑of‑range request */

    static uint64_t next_odd = 1;  /* start from 1, the first odd number */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_odd;
        if (is_unbalanced(cand))
            append_candidate(cand);
        next_odd += 2;           /* skip even numbers */
    }
    return cache[n - 1];
}
