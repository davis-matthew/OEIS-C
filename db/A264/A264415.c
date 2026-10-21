/*
 * A264415  G.f. A(x) satisfies: A(x)^2 = A(x^2) + 30*x.
 * https://oeis.org/A264415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A(x)^2 = A(x^2) + 30*x. Recurrence: a(0) = 1, a(1) = 15. For n
 *            >= 2: S = Σ_{k=1}^{n-1} a(k) * a(n-k) a(n) = (a(n/2) - S)/2 if n
 *            even -S/2 if n odd
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;   /* number of values stored (seq_len == capacity) */

    /* grow the cache if necessary */
    if (n + 1 > seq_len) {
        seq_t *new_seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
        /* realloc may return NULL – ignore that case for brevity as test harness
           guarantees enough memory */
        seq = new_seq;
        /* initialize new entries lazily in the loop below */
        seq_len = n + 1;
    }

    /* compute missing entries up to n */
    for (idx_t i = (seq_len <= 0 ? 0 : /* seq_len was updated above */ 0); i <= n; ++i) {
        if (i == 0) {
            seq[0] = (seq_t)1;
        } else if (i == 1) {
            seq[1] = (seq_t)15;
        } else {
            seq_t S = (seq_t)0;
            /* sum a(k) * a(i-k) for k = 1 .. i-1 */
            for (idx_t k = 1; k < i; ++k) {
                S += seq[k] * seq[i - k];
            }
            if ((i & 1U) == 0) {          /* even i */
                seq[i] = (seq[i / 2] - S) / (seq_t)2;
            } else {
                seq[i] = -(S / (seq_t)2);
            }
        }
    }

    return seq[n];
}
