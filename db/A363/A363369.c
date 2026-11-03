/*
 * A363369  Number of steps x -> x+1 or x/prime required to go from n to 1.
 * https://oeis.org/A363369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=0; for n≥2, a(n)=min(1+a(n+1),1+a(n/p)) over all prime
 *            factors p of n. a(p)=1 if n is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9470 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Auxiliary function: simple trial‑division factorisation; no external
   helper library is required for the OEIS definition. */
static bool is_prime_factor(idx_t n, idx_t *first_factor)
{
    if (n < 2)
        return false;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            if (first_factor)
                *first_factor = p;
            return false; /* composite */
        }
    }
    return true; /* prime */
}

/* Memoised recursive evaluation of the game length. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* out of range – behaviour irrelevant */

    /* Dynamic cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* current capacity (indices 0..cap-1) */

    /* Ensure the cache can hold at least indices n and n+1. */
    if (cache == NULL || n + 1 >= cap) {
        size_t new_cap = (n + 2) * 2;
        if (new_cap < 16)
            new_cap = 16;
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (tmp == NULL)
            return 0; /* allocation failure – not expected in test harness */
        if (cache) {
            mem_copy(tmp, cache, cap * sizeof(seq_t));
            mem_free(cache);
        }
        const seq_t SENTINEL = (seq_t)-1;
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = SENTINEL;
        cache = tmp;
        cap = new_cap;
    }

    const seq_t SENTINEL = (seq_t)-1;
    if (cache[n] != SENTINEL)
        return cache[n];

    /* Base cases. */
    if (n == 1) {
        cache[1] = 0;
        return 0;
    }

    /* Quick prime test: if n is prime, the optimal move is to strip n
       immediately (x/p = 1).  The length then is exactly 1. */
    idx_t dummy = 0;
    if (is_prime_factor(n, &dummy)) {
        cache[n] = 1;
        return 1;
    }

    /* Otherwise, explore all admissible moves. */
    seq_t best = a(n + 1);  /* move “+1” */

    /* Enumerate distinct prime factors of n. */
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t q = n / p;
            seq_t val = a(q);
            if (val < best)
                best = val;
            while (temp % p == 0)
                temp /= p;  /* skip repeated factor */
        }
    }
    if (temp > 1) {          /* remaining prime factor > sqrt(n) */
        idx_t q = n / temp;
        seq_t val = a(q);
        if (val < best)
            best = val;
    }

    seq_t result = best + 1;
    cache[n] = result;
    return result;
}
