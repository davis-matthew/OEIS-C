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
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341938: a(n) = the nth positive integer m such that
   sqrt(phi(m) * tau(m)) is an integer (i.e., phi(m) * tau(m) is a perfect square). */

/* Include required headers. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
        term_vals = mem_resize(seq_t, term_vals, new_cap);
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
        seq_t phi_val = totient_seq(m);
        seq_t tau_val = (seq_t)num_divisors((uint64_t)next_m);

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
