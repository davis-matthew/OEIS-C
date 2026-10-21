/*
 * A258136  Lexicographically earliest sequence of odd positive integers such
 *          that the terms and their absolute first differences are all
 *          distinct.
 * https://oeis.org/A258136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   value_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A258136: the lexicographically earliest
   sequence of odd positive integers such that all terms and all
   their absolute first differences are distinct.
   Computation is performed using a greedy construction. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/value_used.h"

#define MAX_TERMS 200000  /* large enough for n up to ~10 000 */

static seq_t terms[MAX_TERMS];
static seq_t diffs[MAX_TERMS];
static size_t terms_len = 0;
static size_t diffs_len = 0;

/* Initialise the sequence with a(1) = 1. */
static void init_sequence(void) {
    terms[0] = (seq_t)1;
    terms_len = 1;
    diffs_len = 0;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* undefined for n = 0 */

    if (terms_len == 0)
        init_sequence();

    /* Extend the sequence until we have at least n terms. */
    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)1;
        const seq_t last_term = terms[terms_len - 1];

        /* Find the smallest odd candidate that satisfies the
           distinctness constraints. */
        for (;;) {
            if (value_used(terms, terms_len, cand)) {
                cand += (seq_t)2;
                continue;
            }
            seq_t diff = cand > last_term ? cand - last_term : last_term - cand;
            if (value_used(diffs, diffs_len, diff)) {
                cand += (seq_t)2;
                continue;
            }
            break;
        }

        /* Append the new term. */
        if (terms_len >= MAX_TERMS)
            abort(); /* safety check: exceeded preallocated size */
        terms[terms_len++] = cand;

        /* Record the new difference. */
        seq_t diff = cand > last_term ? cand - last_term : last_term - cand;
        if (diffs_len >= MAX_TERMS)
            abort(); /* safety check */
        diffs[diffs_len++] = diff;
    }

    return terms[(size_t)n - 1];
}
