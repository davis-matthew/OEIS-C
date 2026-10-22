/*
 * A306496  Number of (undirected) Hamiltonian cycles in the n-crown graph.
 * https://oeis.org/A306496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A094047(n)/2 where A094047(n) = (n‑1)! * Σ_{m=0}^{n}
 *            (-1)^m * A_m * (n−m)!, and A_m is the number of matchings of
 *            size m in the 2n‑cycle: A_0 = 1, A_m = (2n)/(2n−m) * C(2n−m, m)
 *            for 1 ≤ m ≤ n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The first two terms are zero. */
    if (n <= 2)
        return 0;

    /* n! and (n-1)! */
    seq_t fact_n      = factorial(n);
    seq_t fact_n_minus1 = fact_n / (seq_t)n;   /* exact division */

    /* (n-m)! starting with m = 0 */
    seq_t fact_nm = fact_n;   /* (n-0)! */

    /* Accumulate the alternating sum */
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    idx_t L = 2 * n;   /* length of the 2‑cycle */

    for (idx_t m = 0; m <= n; ++m) {
        seq_t Am;
        if (m == 0) {
            Am = 1;
        } else {
            seq_t bin = binom(L - m, m);          /* C(2n−m, m) */
            seq_t tmp = bin * (seq_t)L;           /* multiply by 2n   */
            Am = tmp / (seq_t)(L - m);            /* exact division  */
        }

        seq_t term = Am * fact_nm;  /* A_m * (n-m)! */

        if ((m & 1) == 0)
            sum_even += term;
        else
            sum_odd  += term;

        /* Prepare (n-(m+1))! for next iteration */
        if (m < n)
            fact_nm /= (seq_t)(n - m);  /* divide by (n-m) */
    }

    /* The directed count D(n) = (n-1)! * (sum_even - sum_odd) */
    seq_t fn = sum_even - sum_odd;
    seq_t directed = fact_n_minus1 * fn;

    /* Each undirected cycle is counted twice */
    return directed / 2;
}
