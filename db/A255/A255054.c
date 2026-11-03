/*
 * A255054  Run lengths in A255072.
 * https://oeis.org/A255054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k that require exactly n steps to *
 *            reach 0 when iterating the map x -> x - (number of runs in
 *            binary * representation of x). The implementation does not use
 *            any external * libraries; it works for any integer type seq_t
 *            and idx_t supplied by * the harness. The function m...
 * Range:     u8 n <= 16143, u16 n <= 16143, u32 n <= 16143, u64 n <= 16143,
 *            u128 n <= 16143, big any n
 * Validated: standard, pass, 145 terms through n = 9734 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
static inline idx_t runs_in_binary(idx_t k) {
    if (k == 0) return 0;          /* not used for k==0 in main loop */
    int runs = 1;                   /* start with the first run */
    int prev_bit = k & 1;
    k >>= 1;
    while (k) {
        int cur_bit = k & 1;
        if (cur_bit != prev_bit) runs++;
        prev_bit = cur_bit;
        k >>= 1;
    }
    return (idx_t)runs;
}

/* ------------------------------------------------------------------ */
/* Helper: compute number of runs in the binary representation of k */

/* ------------------------------------------------------------------ */
/* Global state for memoisation                                    */
/* ------------------------------------------------------------------ */
static idx_t  steps_alloc   = 0;      /* number of allocated step entries   */
static idx_t *steps_arr     = NULL;   /* steps_arr[k] = number of steps for k */

static size_t counts_alloc  = 0;      /* capacity of counts_arr            */
static idx_t *counts_arr    = NULL;   /* counts_arr[s] = number of k with s steps */

static const idx_t FACTOR = 200;     /* safety factor for bounds            */

/* ------------------------------------------------------------------ */
/* Grow steps array up to and including new_limit, updating counts  */
/* ------------------------------------------------------------------ */
static void grow_steps(idx_t new_limit) {
    if (new_limit < steps_alloc) return;          /* already have it */

    /* Reallocate steps array */
    steps_arr = (idx_t*)realloc(steps_arr, (new_limit + 1) * sizeof(idx_t));
    if (!steps_arr) return;                      /* allocation failure */

    /* Compute new entries and update counts */
    for (idx_t k = steps_alloc; k <= new_limit; ++k) {
        if (k == 0) {
            steps_arr[0] = 0;
        } else {
            idx_t runs = runs_in_binary(k);
            idx_t prev = k - runs;
            steps_arr[k] = steps_arr[prev] + 1;
        }

        idx_t s = steps_arr[k];

        /* Ensure counts array can index s */
        if (s >= (idx_t)counts_alloc) {
            size_t new_cap = counts_alloc ? (counts_alloc << 1) : 16;
            while ((size_t)s >= new_cap) new_cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(counts_arr, new_cap * sizeof(idx_t));
            if (!tmp) return; /* allocation failure */
            /* Zero the newly allocated part */
            for (size_t i = counts_alloc; i < new_cap; ++i) tmp[i] = 0;
            counts_arr = tmp;
            counts_alloc = new_cap;
        }

        /* Increment frequency for this step count */
        counts_arr[s]++;
    }

    steps_alloc = new_limit + 1;
}

/* ------------------------------------------------------------------ */
/* Ensure that all required steps up to index n are computed          */
/* ------------------------------------------------------------------ */
static void ensure_computed(idx_t n) {
    idx_t limit = n * FACTOR;
    grow_steps(limit);
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    ensure_computed(n);
    if (n >= (idx_t)counts_alloc) return (seq_t)0;
    return (seq_t)counts_arr[n];
}
