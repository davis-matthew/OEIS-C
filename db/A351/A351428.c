/*
 * A351428  Expansion of e.g.f. 1/exp(exp(exp(exp(exp(x)-1)-1)-1)-1).
 * https://oeis.org/A351428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(n,5) where T(n,k) = sum_{j=0..n} Stirling2(n,j) *
 *            T(j,k-1), k>1, T(n,0) = (-1)^n * n!
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The following function is called for arbitrary n >= 0.
   It uses static arrays to memoise the values of T(n,k)
   for k = 0..5, building them incrementally as larger
   indices are requested.  All arithmetic is performed
   with the type seq_t supplied by the build. */
seq_t a(idx_t n)
{
    /* memoised capacity and tables */
    static idx_t cap = 0;
    static seq_t *T0 = 0, *T1 = 0, *T2 = 0, *T3 = 0, *T4 = 0, *T5 = 0;
    static seq_t *S_prev = 0, *S_curr = 0;
    static seq_t fact_val = 0;          /* factorial of (cap-1) */

    /* Extend tables if needed */
    if (n + 1 > cap) {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;

        /* reallocate all arrays to new_cap */
        T0 = (seq_t *)realloc(T0, sizeof(seq_t) * new_cap);
        T1 = (seq_t *)realloc(T1, sizeof(seq_t) * new_cap);
        T2 = (seq_t *)realloc(T2, sizeof(seq_t) * new_cap);
        T3 = (seq_t *)realloc(T3, sizeof(seq_t) * new_cap);
        T4 = (seq_t *)realloc(T4, sizeof(seq_t) * new_cap);
        T5 = (seq_t *)realloc(T5, sizeof(seq_t) * new_cap);
        S_prev = (seq_t *)realloc(S_prev, sizeof(seq_t) * new_cap);
        S_curr = (seq_t *)realloc(S_curr, sizeof(seq_t) * new_cap);

        /* zero‑initialize new slots */
        if (old_cap < new_cap) {
            for (idx_t i = old_cap; i < new_cap; ++i) {
                T0[i] = T1[i] = T2[i] = T3[i] = T4[i] = T5[i] = 0;
                S_prev[i] = S_curr[i] = 0;
            }
        }

        /* Initialise the first row n = 0 if needed */
        if (old_cap == 0) {
            T0[0] = (seq_t)1;
            T1[0] = (seq_t)1;
            T2[0] = (seq_t)1;
            T3[0] = (seq_t)1;
            T4[0] = (seq_t)1;
            T5[0] = (seq_t)1;
            S_prev[0] = (seq_t)1;          /* Stirling(0,0) = 1 */
            fact_val = (seq_t)1;           /* 0! = 1 */
            cap = 1;
            old_cap = 1;
        }

        /* Build rows from old_cap up to n inclusive */
        for (idx_t curr = old_cap; curr < new_cap; ++curr) {
            /* Compute Stirling numbers S(curr, j) for j = 0..curr */
            if (curr > 0) S_curr[0] = (seq_t)0;
            for (idx_t j = 1; j <= curr; ++j) {
                seq_t t1 = (seq_t)j * S_prev[j];
                seq_t t2 = S_prev[j - 1];
                S_curr[j] = t1 + t2;
            }

            /* Factorial: previous is (curr-1)!
               Multiply by curr to get curr! */
            fact_val = fact_val * (seq_t)curr;

            /* T0[n] = (-1)^n * n! */
            seq_t sign = ((curr & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            T0[curr] = sign * fact_val;

            /* Now compute T1..T5 for this row */
            seq_t sum;

            /* T1 */
            sum = (seq_t)0;
            for (idx_t j = 0; j <= curr; ++j) sum += S_curr[j] * T0[j];
            T1[curr] = sum;

            /* T2 */
            sum = (seq_t)0;
            for (idx_t j = 0; j <= curr; ++j) sum += S_curr[j] * T1[j];
            T2[curr] = sum;

            /* T3 */
            sum = (seq_t)0;
            for (idx_t j = 0; j <= curr; ++j) sum += S_curr[j] * T2[j];
            T3[curr] = sum;

            /* T4 */
            sum = (seq_t)0;
            for (idx_t j = 0; j <= curr; ++j) sum += S_curr[j] * T3[j];
            T4[curr] = sum;

            /* T5 */
            sum = (seq_t)0;
            for (idx_t j = 0; j <= curr; ++j) sum += S_curr[j] * T4[j];
            T5[curr] = sum;

            /* Prepare for next iteration: swap S_prev and S_curr */
            seq_t *tmp = S_prev;
            S_prev = S_curr;
            S_curr = tmp;
        }

        cap = new_cap;
    }

    return T5[n];
}
