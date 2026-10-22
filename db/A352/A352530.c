/*
 * A352530  Expansion of 1/Sum_{k>=0} x^(k^5).
 * https://oeis.org/A352530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n > 0: a(n) = – Σ_{k≥1, k^5 ≤ n} a(n – k^5)
 * Range:     i8 n <= 81, i16 n <= 163, i32 n <= 308, i64 n <= 627,
 *            i128 n <= 1231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes the coefficient array and the list of fifth
   powers that are needed for the recurrence.  The implementation is
   O(limit^(1/5)) for computing up to a given n.  It works for the
   arbitrary‑precision build as well as for the 64‑bit integer build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- Memoised coefficient array ---------- */
    static seq_t *coeff = NULL;   /* coeff[i] = a(i)          */
    static size_t cap   = 0;      /* allocated size           */
    static idx_t  maxn  = 0;      /* highest n that has been
                                     computed (≥ 0)            */

    /* ---------- Memoised list of fifth powers ---------- */
    static idx_t *pow5 = NULL;    /* pow5[j] = (j+1)^5        */
    static size_t pw_cap = 0;     /* allocated capacity       */
    static idx_t  pw_len  = 0;    /* number of powers stored  */

    /* ----------- Ensure coefficient array capacity ----- */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap : 64;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        coeff = (seq_t*)realloc(coeff, new_cap * sizeof(seq_t));
        /* initialise new entries to zero */
        for (size_t i = cap; i < new_cap; ++i) coeff[i] = 0;
        cap = new_cap;
    }

    /* ----------- Initialise a(0) ----- */
    coeff[0] = 1;   /* base case needed for all calls          */

    /* ----------- Extend list of fifth powers up to n ---- */
    if (pw_cap == 0) {
        pw_cap = 64;
        pow5   = (idx_t*)malloc(pw_cap * sizeof(idx_t));
    }
    /* compute next powers while the next power (k = pw_len+1)
       is ≤ n.  We generate one more power than strictly needed
       but the extra cost is negligible for n <= 10 000.      */
    for (idx_t k = pw_len + 1;
         (idx_t)(k * k * k * k * k) <= n;
         ++k) {
        if (pw_len == (idx_t)pw_cap) {
            pw_cap <<= 1;
            pow5 = (idx_t*)realloc(pow5, pw_cap * sizeof(idx_t));
        }
        pow5[pw_len++] = k * k * k * k * k;
    }

    /* ----------- Compute coefficients up to n (if needed) ----- */
    if (n > maxn) {
        for (idx_t i = maxn + 1; i <= n; ++i) {
            /* a(i) = – Σ_{k} a(i – k^5)   */
            seq_t s = 0;
            for (idx_t j = 0; j < pw_len; ++j) {
                idx_t p = pow5[j];
                if (p > i) break;
                s += coeff[i - p];
            }
            coeff[i] = -s;
        }
        maxn = n;
    }

    /* ----------- Return requested term ---------- */
    return coeff[n];
}
