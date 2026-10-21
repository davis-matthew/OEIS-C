/*
 * A292794  Numbers not congruent to A000045(k) mod A000045(k+1) for all k >
 *          1.
 * https://oeis.org/A292794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth non‑negative integer that is not congruent
   to fib(k) modulo fib(k+1) for any k>1 (k > 1). */
/* OEIS notation: a(n) = smallest integer m ≥ 0 such that
   ∀k>1: m % F(k+1) ≠ F(k). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq = NULL;
static size_t seq_size = 0, seq_cap = 0;

static seq_t *fib = NULL;
static size_t fib_size = 0, fib_cap = 0;

/* Ensure that seq has at least 'min_cap' capacity. */
static void seq_reserve(size_t min_cap) {
    if (min_cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *newp = (seq == NULL) ? seq_alloc(new_cap) : seq_realloc(seq, seq_cap, new_cap);
    if (!newp) exit(EXIT_FAILURE);
    seq = newp;
    seq_cap = new_cap;
}

/* Ensure that fib has at least 'min_cap' capacity. */
static void fib_reserve(size_t min_cap) {
    if (min_cap <= fib_cap) return;
    size_t new_cap = fib_cap ? fib_cap * 2 : 1;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *newp = (fib == NULL) ? seq_alloc(new_cap) : seq_realloc(fib, fib_cap, new_cap);
    if (!newp) exit(EXIT_FAILURE);
    fib = newp;
    fib_cap = new_cap;
}

/* Grow the Fibonacci table so that fib[i] > limit.  At least one element
   greater than 'limit' is guaranteed after return. */
static void ensure_fib_for(seq_t limit) {
    /* initialize fib[0] = fib[1] = 1 */
    while (fib_size < 2) {
        fib_reserve(fib_size + 1);
        if (fib_size == 0)
            fib[fib_size++] = (seq_t)1;
        else if (fib_size == 1)
            fib[fib_size++] = (seq_t)1;
    }
    /* extend until the last Fibonacci number is > limit */
    while (fib_size > 0 && fib[fib_size - 1] <= limit) {
        fib_reserve(fib_size + 1);
        seq_t next = fib[fib_size - 1] + fib[fib_size - 2];
        fib[fib_size++] = next;
    }
}

/* Initialize the sequence with a(0) = 0. */
static void ensure_initialized(void) {
    if (seq_size != 0) return;
    seq_reserve(1);
    seq[0] = (seq_t)0;
    seq_size = 1;
}

/* Return the nth term of OEIS A292794. */
seq_t a(idx_t n) {
    ensure_initialized();

    while (seq_size <= n) {
        seq_t candidate = seq[seq_size - 1] + (seq_t)1;
        for (;;) {
            ensure_fib_for(candidate);
            bool bad = false;
            for (idx_t i = 1; i + 1 < fib_size && fib[i] <= candidate; ++i) {
                if (candidate % fib[i + 1] == fib[i]) {
                    bad = true;
                    break;
                }
            }
            if (!bad)
                break;          /* candidate accepted */
            ++candidate;         /* try next integer */
        }
        seq_reserve(seq_size + 1);
        seq[seq_size++] = candidate;
    }
    return seq[n];
}
