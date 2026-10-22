/*
 * A336636  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(BesselI(0,2*sqrt(x))^3 - 1).
 * https://oeis.org/A336636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 1,
 * a(n) = (1/n) * Σ_{k=1..n} binomial(n,k)^2 * A002893(k) * k * a(n-k)
 *   with A002893(k) = Σ_{i=0..k} binomial(k,i)^2 * binomial(2(k-i), k-i)
 *
 * This implementation precomputes the required A002893(k) values up to n
 * and then evaluates the recurrence in O(n^2) time, using only integer
 * arithmetic on the provided seq_t type.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate temporary arrays */
    seq_t *g   = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* A002893 values */
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* a(n) values */
    seq_t *S   = (seq_t *)malloc((n + 1) * sizeof(seq_t)); /* S[m] = C(2m,m) */

    if (!g || !a_arr || !S) {
        /* Allocation failed – return 0 (should not happen in tests) */
        if (g) free(g);
        if (a_arr) free(a_arr);
        if (S) free(S);
        return (seq_t)0;
    }

    /* Initial values */
    g[0] = (seq_t)0;
    a_arr[0] = (seq_t)1;
    S[0] = (seq_t)1;

    /* Precompute S[m] = C(2m,m) for m = 0..n */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t num = S[m - 1] * ( (seq_t)(2 * m) * (seq_t)(2 * m - 1) );
        seq_t den = (seq_t)m * (seq_t)m;
        S[m] = num / den;          /* exact division */
    }

    /* Compute A002893(k) = g[k] for k = 1..n */
    for (idx_t curr = 1; curr <= n; ++curr) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* C(curr,0) */
        for (idx_t i = 0; i <= curr; ++i) {
            seq_t binom_sq = binom * binom;
            seq_t term = binom_sq * S[curr - i];
            sum += term;
            if (i < curr) {
                binom = binom * (seq_t)(curr - i);
                binom = binom / (seq_t)(i + 1);   /* next binomial */
            }
        }
        g[curr] = sum;
    }

    /* Evaluate the recurrence for a[n] */
    for (idx_t curr = 1; curr <= n; ++curr) {
        seq_t sum = (seq_t)0;
        seq_t binom_curr = (seq_t)1;   /* for k=0 */
        for (idx_t k = 1; k <= curr; ++k) {
            /* Update binomial(curr,k) */
            binom_curr = binom_curr * (seq_t)(curr - k + 1);
            binom_curr = binom_curr / (seq_t)k;

            seq_t binom_sq = binom_curr * binom_curr;
            seq_t term = binom_sq * g[k];
            term = term * (seq_t)k;
            term = term * a_arr[curr - k];
            sum += term;
        }
        a_arr[curr] = sum / (seq_t)curr;
    }

    seq_t result = a_arr[n];

    free(g);
    free(a_arr);
    free(S);
    return result;
}
