/*
 * A350467  a(n) = hypergeom([1/2 - n/2, -n/2], [-n], -8*n).
 * https://oeis.org/A350467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-k, k) * (2*n)^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence
 *   binom_{k+1} = binom_k * (n-2k)*(n-2k-1) / ((n-k)*(k+1))
 * to compute all terms in O(n) time. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Prepare variables in seq_t to avoid overflow. */
    seq_t sum = 0;
    seq_t term_pow = 1;          /* (2*n)^k      */
    seq_t binom = 1;             /* binomial(n-k, k)  */
    idx_t maxk = n / 2;          /* only up to floor(n/2) non‑zero terms */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Add current term to the sum. */
        sum += binom * term_pow;

        /* Stop when the last term has been added. */
        if (k == maxk)
            break;

        /* Update (2*n)^k to (2*n)^(k+1). */
        term_pow *= (seq_t)(2 * n);

        /* Update the binomial coefficient for the next k. */
        idx_t num1 = n - 2 * k;          /* n-2k   */
        idx_t num2 = n - 2 * k - 1;      /* n-2k-1 */
        idx_t den1 = n - k;              /* n-k    */
        idx_t den2 = k + 1;              /* k+1    */

        binom = binom * (seq_t)num1;
        binom = binom * (seq_t)num2;
        binom = binom / ((seq_t)den1 * (seq_t)den2);
    }

    return sum;
}
