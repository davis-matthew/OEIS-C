/*
 * A258277  Expansion of chi(-q) * phi(-q^3) * psi(q^3) in powers of q where
 *          chi(), phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A258277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of Q^(-1/3) * eta(Q) * eta(Q^3) * eta(Q^6) / eta(Q^2)
   via the product:  ∏_{k>0} (1 - x^{3k}) (1 - x^{6k}) / (1 + x^k). */
#include <stddef.h>
#include <stdlib.h>

/* The build supplies the following routines for handling seq_t arrays.
   They are sufficient for allocating and freeing the coefficient buffer. */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t *p); */

seq_t a(idx_t n) {
    /* Work with a size_t extent for loop counters. */
    const size_t N = (size_t)n;
    /* Allocate coefficient array 0..N, initialise with buf[0] = 1. */
    seq_t *buf = alloc_seq_array(N + 1);
    if (!buf) return 0;          /* allocation failure – unlikely in test harness. */
    buf[0] = 1;
    for (size_t i = 1; i <= N; ++i) {
        buf[i] = 0;
    }

    /* Iterate over the factorising product. */
    for (size_t k = 1; k <= N; ++k) {
        /* Multiply by (1 - x^{3k}) if the shift does not exceed N. */
        const size_t shift3 = 3 * k;
        if (shift3 <= N) {
            for (size_t i = N; i >= shift3; --i) {
                buf[i] -= buf[i - shift3];
            }
        }

        /* Multiply by (1 - x^{6k}) if the shift does not exceed N. */
        const size_t shift6 = 6 * k;
        if (shift6 <= N) {
            for (size_t i = N; i >= shift6; --i) {
                buf[i] -= buf[i - shift6];
            }
        }

        /* Divide by (1 + x^k).  For each i≥k : new = old - new_{i-k}. */
        for (size_t i = k; i <= N; ++i) {
            buf[i] -= buf[i - k];
        }
    }

    seq_t result = buf[N];
    free_seq_array(buf);
    return result;
}
