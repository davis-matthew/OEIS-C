/*
 * A360854  Number of induced cycles in the n X n rook graph.
 * https://oeis.org/A360854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*binomial(n,3) + sum_{k=2..n} binomial(n,k)^2 * k! *
 *            (k-1)! / 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t result = 0;

    /* Triangle term */
    if (n >= 3) {
        seq_t tri = binomial(n, 3);
        result += 2 * ((seq_t)n) * tri;
    }

    seq_t binom = 1;          /* C(n,0)  */
    seq_t fact_prev = 1;      /* (k-1)!  */
    seq_t fact_curr = 1;      /* k!      */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial to C(n,k) */
        binom = binom * (n - k + 1) / k;

        if (k >= 2) {
            seq_t binom_sq   = binom * binom;
            seq_t fact_half  = fact_curr / 2;      /* k! / 2, k >= 2 guarantees divisibility */
            seq_t term       = binom_sq * fact_prev;
            term *= fact_half;
            result += term;
        }

        /* Prepare factorials for next k */
        if (k < n) {
            seq_t next_fact = fact_curr * (k + 1); /* (k+1)!  */
            fact_prev = fact_curr;
            fact_curr = next_fact;
        }
    }

    return result;
}
