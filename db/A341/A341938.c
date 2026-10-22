/*
 * A341938  Numbers m such that the geometric mean of tau(m) and phi(m) is an
 *          integer where phi is the Euler totient function (A000010) and tau
 *          is the number of divisors function (A000005).
 * https://oeis.org/A341938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   calc_phi, divisor_count_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341938: a(n) = the nth positive integer m such that
   sqrt(phi(m) * tau(m)) is an integer (i.e., phi(m) * tau(m) is a perfect square). */

/* Include required headers. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/calc_phi.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/floor_sqrt.h"

/* Static storage for memoizing computed sequence values. */
static seq_t *term_vals = NULL;          /* array of found terms */
static size_t term_cap = 0;              /* current capacity of term_vals */
static size_t term_len = 0;              /* number of terms stored */
static idx_t  next_m   = 1;              /* next integer to test (1‑based) */

/* Grow the terms array if needed and append a new term. */
static void append_term(seq_t v)
{
    if (term_len == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 8;
        term_vals = (seq_t *)realloc(term_vals, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }
    /* Store the new term and increment the length. */
    term_vals[term_len++] = v;
}

/* Main sequence function.  Computes terms lazily and records them in a
   static array so that subsequent calls can be answered directly. */
seq_t a(idx_t n)
{
    /* Ensure that at least n terms have been found. */
    while (term_len < n) {
        /* Candidate m to test. */
        seq_t m = (seq_t)next_m;
        /* Compute phi(m) and tau(m).  For tau we use the 64‑bit routine
           because m comfortably fits in uint64_t. */
        seq_t phi_val = calc_phi(m);
        seq_t tau_val = (seq_t)divisor_count_u64((uint64_t)next_m);

        /* Check if the product is a perfect square. */
        seq_t prod = phi_val * tau_val;
        seq_t s    = floor_sqrt(prod);
        if (s * s == prod) {
            append_term(m);                 /* record as a valid term */
        }
        ++next_m;                            /* proceed to next integer */
    }

    /* Return the requested term (index 1 → term_vals[0]). */
    return term_vals[n - 1];
}
