/*
 * A256420  a(n) = 2n unless n is prime, in which case a(n) = first term not
 *          yet present in the sequence.
 * https://oeis.org/A256420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n unless n is prime, in which case a(n) = the smallest
 *            integer not yet used in the sequence
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Global state for memoisation */
static seq_t *a_vals   = NULL;   /* 1‑based values stored as 0‑based */
static idx_t  a_cap    = 0;      /* capacity of a_vals */
static idx_t  computed = 0;      /* highest n computed */

static bool *used   = NULL;   /* bitset of used integers, index 0 unused */
static idx_t used_cap = 0;    /* capacity of used */

/* Smallest unused positive integer, maintained across calls */
static idx_t next_missing = 1;

/* Ensure enough space in the used bitset for index `needed` (0‑based). */
static void ensure_used_cap(idx_t needed) {
    if (used_cap > needed) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, (size_t)new_cap * sizeof(bool));
    if (!new_used) abort();                 /* memory exhaustion, unlikely */
    memset(new_used + used_cap, 0,
           (size_t)(new_cap - used_cap) * sizeof(bool));
    used     = new_used;
    used_cap = new_cap;
}

/* Ensure enough space in the a_vals array for index `needed` (0‑based). */
static void ensure_a_cap(idx_t needed) {
    if (a_cap > needed) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_vals = (seq_t *)realloc(a_vals,
                                      (size_t)new_cap * sizeof(seq_t));
    if (!new_vals) abort();
    a_vals = new_vals;
    a_cap  = new_cap;
}

/* Extend the sequence up to `upto` (inclusive). */
static void extend_up_to(idx_t upto) {
    for (idx_t n = computed + 1; n <= upto; ++n) {
        seq_t val;
        if (is_prime(n)) {
            /* Find the smallest unused positive integer. */
            while (true) {
                if (next_missing >= used_cap) ensure_used_cap(next_missing);
                if (!used[(size_t)next_missing]) break;
                ++next_missing;
            }
            val = (seq_t)next_missing;
            used[(size_t)next_missing] = true;
            ++next_missing;                 /* advance for the next prime */
        } else {
            val = (seq_t)n * (seq_t)2;
            idx_t idx_val = (idx_t)val;      /* 2n fits in idx_t for our bounds */
            if (idx_val >= used_cap) ensure_used_cap(idx_val);
            used[(size_t)idx_val] = true;
        }
        ensure_a_cap(n - 1);
        a_vals[n - 1] = val;
    }
    computed = upto;
}

/* The required sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* behaviour below offset irrelevant */
    if (n > computed) extend_up_to(n);
    return a_vals[n - 1];
}
