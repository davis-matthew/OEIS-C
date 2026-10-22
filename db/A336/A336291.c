/*
 * A336291  a(n) = (n!)^2 * Sum_{k=1..n} 1 / (k * ((n-k)!)^2).
 * https://oeis.org/A336291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=1..n} 1 / (k * ((n-k)!)^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes A336291: a(n) = Sum_{k=1..n} (k-1)! * k! * binomial(n,k)^2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;          /* running total of the sum */
    seq_t binom = 1;        /* C(n,0) = 1, will be updated to C(n,k) each loop */
    seq_t fact_prev = 1;    /* (k-1)! for the current k, starting with k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute k! from (k-1)! */
        seq_t fact_k = fact_prev * (seq_t)k;

        /* update binomial coefficient to C(n,k) */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* term = (k-1)! * k! * C(n,k)^2 */
        seq_t term = fact_prev * fact_k;
        term = term * binom * binom;

        sum += term;
        fact_prev = fact_k;  /* next iteration will need (k)! as (k-1)! */
    }

    return sum;
}
