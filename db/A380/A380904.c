/*
 * A380904  An integer sequence giving a counterexample to a theorem of Szüsz
 *          and Volkmann.
 * https://oeis.org/A380904
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 745, u32 n <= 10009, u64 n <= 10009,
 *            u128 n <= 10009, big any n
 * Validated: standard, pass, 128 terms through n = 9964 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380904
   Formula: Recurrence
   a(1) = 0
   For n > 1:
       h(n) = 0 if n < 10
              else 10^floor(log10(n))  (the largest power of 10 ≤ n)
       if n > h(n):
           a(n) = a(n-1) + h(n)
       else:
           a(n) = n * floor((a(n-1)+h(n))/n)
   All computations are performed with type seq_t; no floating point is used. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)0;            /* a(1) will be set to 0 by the loop logic */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t h;
        if (i < 10) {
            h = (seq_t)0;
        } else {
            int d = num_digits_idx(i);                 /* number of decimal digits of i */
            h = pow10(d - 1);                  /* 10^(d-1)  */
        }

        if ((seq_t)i > h) {
            a_val = a_val + h;
        } else {
            seq_t sum = a_val + h;
            seq_t div = sum / (seq_t)i;        /* floor division, truncating */
            a_val = (seq_t)i * div;
        }
    }

    return a_val;
}
