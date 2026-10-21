/*
 * A285408  Expansion of 1/(1 + x/(1 + x^4/(1 + x^9/(1 + x^16/(1 + x^25/(1 +
 *          ... + x^(k^2)/(1 + ...))))))), a continued fraction.
 * https://oeis.org/A285408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of xⁿ in the power‑series expansion of 1/(1 +
 *            x/(1 + x⁴/(1 + x⁹/(1 + x¹⁶/(1 + ...))))) . The series is
 *            obtained by the convergents P_k, Q_k: P_k = P_{k-1} + x^{k²}
 *            P_{k-2} Q_k = Q_{k-1} + x^{k²} Q_{k-2} with P_{-1}=1, P_0=1;
 *            Q_{-1}=0, Q_0=1. Then a(x) = Q(x) / P(x) as a power s...
 * Range:     i8 n <= 33, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   ceil_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ceil_sqrt_u64.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Size of coefficient arrays = n+1.  */
    size_t cap = (size_t)(n + 1);

    /* Allocate polynomial arrays for P and Q.  */
    seq_t *P_prev2 = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *P_prev1 = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *Q_prev2 = (seq_t *)calloc(cap, sizeof(seq_t));
    seq_t *Q_prev1 = (seq_t *)calloc(cap, sizeof(seq_t));

    /* Base values:  P_{-1}=1, P_0=1 ;  Q_{-1}=0, Q_0=1.  */
    if (!P_prev2 || !P_prev1 || !Q_prev2 || !Q_prev1) {
        /* Allocation failed – abort.  */
        return 0;
    }
    P_prev2[0] = (seq_t)1;
    P_prev1[0] = (seq_t)1;
    Q_prev2[0] = (seq_t)0;
    Q_prev1[0] = (seq_t)1;

    /* Number of layers needed to cover exponents up to n.  */
    idx_t layers = ceil_sqrt_u64(n);

    for (idx_t k = 1; k <= layers; ++k) {
        idx_t shift = k * k;                     /* k² */
        seq_t *P_new = (seq_t *)calloc(cap, sizeof(seq_t));
        seq_t *Q_new = (seq_t *)calloc(cap, sizeof(seq_t));

        if (!P_new || !Q_new) {
            /* Allocation failed – free and abort.  */
            free(P_prev2); free(P_prev1);
            free(Q_prev2); free(Q_prev1);
            return 0;
        }

        /* Copy the “1 + …” part (a_k = 1).  */
        for (size_t i = 0; i < cap; ++i) {
            P_new[i] = P_prev1[i];
            Q_new[i] = Q_prev1[i];
        }

        /* Add the shifted part if it fits within the range.  */
        if (shift <= n) {
            size_t s = (size_t)shift;
            for (size_t i = s; i < cap; ++i) {
                P_new[i] += P_prev2[i - s];
                Q_new[i] += Q_prev2[i - s];
            }
        }

        /* Advance the recurrence.  */
        free(P_prev2); free(Q_prev2);
        P_prev2 = P_prev1;  Q_prev2 = Q_prev1;
        P_prev1 = P_new;    Q_prev1 = Q_new;
    }

    /* At this point P_prev1 = P_final, Q_prev1 = Q_final.  */
    /* Perform series division: a = Q / P, where P[0] == 1.  */
    seq_t *a_coeff = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!a_coeff) {
        /* Allocation failed – free resources.  */
        free(P_prev2); free(P_prev1);
        free(Q_prev2); free(Q_prev1);
        return 0;
    }

    /* a[0] = Q[0] / P[0] = Q[0] (since P[0]==1).  */
    a_coeff[0] = Q_prev1[0];

    for (size_t i = 1; i < cap; ++i) {
        seq_t sum = 0;
        /* sum_{j=1..i} a[i-j] * P[j]  */
        for (size_t j = 1; j <= i; ++j) {
            sum += a_coeff[i - j] * P_prev1[j];
        }
        a_coeff[i] = Q_prev1[i] - sum;
    }

    seq_t result = a_coeff[n];

    /* Clean up.  */
    free(P_prev2); free(P_prev1);
    free(Q_prev2); free(Q_prev1);
    free(a_coeff);

    return result;
}
