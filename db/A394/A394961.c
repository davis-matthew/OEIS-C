/*
 * A394961  a(n) = n! * [x^n] (exp(exp(x) - 1) - 1)/(1 - x).
 * https://oeis.org/A394961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   bell_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = n * a(n‑1) + Bell(n),  a(0) = 0
   where Bell(n) = Σ_{k=1..n} S(n,k) and S(n,k) = k*S(n‑1,k) + S(n‑1,k‑1),
   S(0,0) = 1, all other S(0,k)=0.  This implements sequence A394961. */

#include <stdlib.h>
#include "../utils/bell_n.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* a(0) = 0 */

    size_t sz = (size_t)(n + 1);
    seq_t *S_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *S_cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!S_prev || !S_cur) {                 /* allocation failed */
        free(S_prev);
        free(S_cur);
        return (seq_t)0;
    }

    S_prev[0] = (seq_t)1;                     /* S(0,0)=1   */
    seq_t a_val = (seq_t)0;                   /* a(0) = 0   */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t bell_i = bell_n(i, S_prev, S_cur);
        a_val        = a_val * (seq_t)i + bell_i;
        /* swap rows for next iteration */
        seq_t *tmp   = S_prev;
        S_prev       = S_cur;
        S_cur        = tmp;
    }

    free(S_prev);
    free(S_cur);
    return a_val;
}
