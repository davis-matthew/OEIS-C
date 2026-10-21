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
 * Helpers:   digit_sum, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the integers n such that the cumulative sum of
 * the decimal digit sums of all numbers from 1 to n is a prime.
 * This implementation builds the sequence on demand and caches previously
 * found terms. */
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/isPrimeGeneric.h"

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
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
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
