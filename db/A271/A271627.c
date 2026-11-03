/*
 * A271627  Numbers n such that the sum of the digits of the numbers from 1 to
 *          n is a prime.
 * https://oeis.org/A271627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9745 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers n such that the cumulative sum of
 * the decimal digit sums of all numbers from 1 to n is a prime.
 * This implementation builds the sequence on demand and caches previously
 * found terms. */
#include <stdbool.h>
#include <stdlib.h>

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
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

static seq_t *seq_cache = NULL;
static size_t seq_cnt = 0;
static size_t seq_cap = 0;
static idx_t    currentX   = 0;
static seq_t    cumSum     = 0;
static bool     initialized = false;

/* Ensure the cache has space for at least one more element. */
static void ensure_cache_capacity(void) {
    if (seq_cnt >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp) return; /* allocation failed – fall back to previous */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* Return the nth element of the sequence (a(1) is the first term). */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of range, not used by tests */

    if (!initialized) {
        /* Initial state: nothing cached yet */
        seq_cache   = NULL;
        seq_cap     = 0;
        seq_cnt     = 0;
        currentX    = 0;
        cumSum      = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n terms cached */
    while (seq_cnt < n) {
        currentX++;                                 /* next natural number */
        seq_t dsum = digit_sum((seq_t)currentX);   /* sum of its digits   */
        cumSum += dsum;                             /* update cumulative sum */
        if (isPrimeGeneric(cumSum)) {               /* check primality     */
            ensure_cache_capacity();
            seq_cache[seq_cnt++] = currentX;        /* store new term      */
        }
    }

    return seq_cache[n - 1];                       /* 0‑based indexing   */
}
