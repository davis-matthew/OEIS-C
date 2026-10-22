/*
 * A366095  Expansion of (1/x) * Series_Reversion( x*(1+x-x^3)/(1+x)^2 ).
 * https://oeis.org/A366095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(n+k,k) *
 *            binomial(n-k+1,n-3*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements this definition and uses only integer arithmetic. */

#include <stdbool.h>   /* needed for bool if any helpers use it */

seq_t a(idx_t n) {
    idx_t max_k = n / 3;
    seq_t sum   = (seq_t)0;
    seq_t C1    = (seq_t)1;                       /* binomial(n+k,k) with k=0 */
    seq_t C2    = (seq_t)(n + 1);                 /* binomial(n-k+1,n-3k) with k=0 */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = C1 * C2;
        sum += term;

        if (k == max_k) break;                  /* no more updates needed */

        /* Update C1: C1(new) = C1(old) * (n + k + 1) / (k + 1) */
        C1 = C1 * ( (seq_t)(n) + (seq_t)k + (seq_t)1 ) / ( (seq_t)k + (seq_t)1 );

        /* Update C2: C2(new) = C2(old) * (n-3k)(n-3k-1)(n-3k-2) /
                                 ((n-k+1)(2k+2)(2k+3)) */
        seq_t num = (seq_t)(n - 3 * k) * (seq_t)(n - 3 * k - 1) *
                    (seq_t)(n - 3 * k - 2);
        seq_t den = (seq_t)(n - k + 1) * (seq_t)(2 * k + 2) *
                    (seq_t)(2 * k + 3);
        C2 = C2 * num / den;
    }

    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
