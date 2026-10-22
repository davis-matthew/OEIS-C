/*
 * A380904  An integer sequence giving a counterexample to a theorem of Szüsz
 *          and Volkmann.
 * https://oeis.org/A380904
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 745, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   digits, pow10
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

#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */
#include "../utils/digits.h"  /* int digits(idx_t x); */

seq_t a(idx_t n)
{
    seq_t a_val = (seq_t)0;            /* a(1) will be set to 0 by the loop logic */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t h;
        if (i < 10) {
            h = (seq_t)0;
        } else {
            int d = digits(i);                 /* number of decimal digits of i */
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
