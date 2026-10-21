/*
 * A280290  Numbers n such that number of partitions of n is even and number
 *          of partitions of n into distinct parts is even.
 * https://oeis.org/A280290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that p(n) (ordinary partitions) is even AND d(n)
 * (partitions into distinct parts) is even.  
 * The sequence is formed by scanning integers in increasing order, testing
 * the two parity conditions, and selecting the qualifying numbers.
 * The implementation precalculates partition parities up to the needed limit
 * using Euler’s pentagonal recurrence and uses the known property that
 * d(n) is odd iff n is a generalized pentagonal number.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *matches = NULL;    /* array of qualifying numbers */
static idx_t   matches_len = 0;  /* number of elements stored */
static idx_t   matches_cap = 0;  /* allocated capacity of matches */
static idx_t   max_n_cached = 0; /* maximum n up to which parities were computed */

/* Helper to append a value to matches */
static void append_match(seq_t val) {
    if (matches_len == matches_cap) {
        size_t new_cap = matches_cap ? matches_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(matches, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – aborting as this should not happen. */
            abort();
        }
        matches = tmp;
        matches_cap = new_cap;
    }
    matches[matches_len++] = val;
}

/* Build parities and pentagonal marker array up to 'upto'. */
static void build_to(idx_t upto) {
    /* Allocate temporary arrays for parity and pentagonal markers. */
    uint8_t *parity = (uint8_t *)calloc((size_t)upto + 1, sizeof(uint8_t));
    uint8_t *is_pent = (uint8_t *)calloc((size_t)upto + 1, sizeof(uint8_t));
    if (!parity || !is_pent) abort();

    /* Mark generalized pentagonal numbers. */
    for (idx_t k = 1;; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;
        if (g1 > upto && g2 > upto) break;
        if (g1 <= upto) is_pent[g1] = 1;
        if (g2 <= upto) is_pent[g2] = 1;
    }

    /* Compute ordinary partition parity using Euler’s recurrence. */
    parity[0] = 1; /* p(0) = 1 is odd */
    for (idx_t n = 1; n <= upto; ++n) {
        uint8_t val = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            val ^= parity[n - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= n) val ^= parity[n - g2];
        }
        parity[n] = val; /* 0 → even, 1 → odd */
    }

    /* Append qualifying numbers to matches.  Start from max_n_cached + 1. */
    for (idx_t n = (max_n_cached + 1) ? max_n_cached + 1 : 1; n <= upto; ++n) {
        /* d(n) is even iff n is NOT a pentagonal number. */
        if (parity[n] == 0 && !is_pent[n]) {
            append_match((seq_t)n);
        }
    }

    max_n_cached = upto;
    free(parity);
    free(is_pent);
}

seq_t a(idx_t n) {
    /* Ensure we have at least n matching terms computed. */
    idx_t limit = max_n_cached ? max_n_cached : 1;
    while (matches_len < n) {
        build_to(limit);
        if (matches_len >= n) break;
        limit *= 2;
    }
    return matches[n - 1];
}
