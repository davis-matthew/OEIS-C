/*
 * A292869  Expansion of 1/(1 + x + x^2 + x^3/(1 + x^4 + x^5 + x^6/(1 + x^7 +
 *          x^8 + x^9/(1 + ...)))), a continued fraction.
 * https://oeis.org/A292869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 52, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292869: Coefficients of the power series of
   1 / (1 + x + x^2 + x^3/(1 + x^4 + x^5 + x^6/(1 + x^7 + x^8 + x^9/(1 + ...)))). */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* For n = 0 return the constant term immediately. */
    if (n == 0) return (seq_t)1;

    /* Working degree.  We only need coefficients up to n. */
    size_t N = (size_t)n;
    size_t sz = N + 1;               /* number of coefficients */
    size_t i, j;

    /* Allocate temporary series arrays.  Three of them are reused during the
       computation.  T_next holds the current sub‑fraction,
       inv holds the inverse of T_next, and T_curr is the series for the
       higher‑level fraction. */
    seq_t *T_next = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *inv    = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *T_curr = (seq_t *)calloc(sz, sizeof(seq_t));

    if (!T_next || !inv || !T_curr) {
        /* Allocation failed – return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Base of the recursion: the deepest sub‑fraction is simply 1. */
    T_next[0] = (seq_t)1;   /* constant term */

    /* Determine the highest index of the block we need to handle.
       A block starts at exponent 3k+1, so we need k with 3k+1 <= N. */
    size_t Kmax = (N >= 1) ? (N - 1) / 3 : 0;

    for (i = Kmax;; --i) {
        /* Compute inv = 1 / T_next modulo x^(N+1).
           Because T_next[0] == 1, the inversion formula simplifies. */
        inv[0] = (seq_t)1;
        for (j = 1; j <= N; ++j) {
            seq_t s = (seq_t)0;
            for (size_t k = 1; k <= j; ++k) {
                s += T_next[k] * inv[j - k];
            }
            inv[j] = -s;
        }

        /* Build the next fractional level T_curr. */
        for (j = 0; j <= N; ++j) T_curr[j] = (seq_t)0;
        T_curr[0] = (seq_t)1;       /* constant term */

        size_t p1 = 3 * i + 1;
        size_t p2 = 3 * i + 2;
        size_t p3 = 3 * i + 3;

        if (p1 <= N) T_curr[p1] = (seq_t)1;
        if (p2 <= N) T_curr[p2] = (seq_t)1;

        /* Add the shifted inverse series: x^(p3) * inv. */
        if (p3 <= N) {
            size_t limit = N - p3;
            for (j = 0; j <= limit; ++j) {
                T_curr[j + p3] += inv[j];
            }
        }

        /* Prepare for the next iteration.  Swap pointers to avoid copying. */
        seq_t *tmp = T_next;
        T_next = T_curr;
        T_curr = tmp;

        if (i == 0) break;
    }

    /* Finally compute the inverse of T_next to obtain the required series
       F(x) = 1 / T_0(x).  The coefficient a(n) is the n-th coefficient
       of this series. */
    inv[0] = (seq_t)1;
    for (j = 1; j <= N; ++j) {
        seq_t s = (seq_t)0;
        for (size_t k = 1; k <= j; ++k) {
            s += T_next[k] * inv[j - k];
        }
        inv[j] = -s;
    }

    seq_t result = inv[N];

    /* Free temporary memory. */
    free(T_next);
    free(inv);
    free(T_curr);

    return result;
}
