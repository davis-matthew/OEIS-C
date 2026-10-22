/*
 * A362393  E.g.f. satisfies A(x) = exp(x + x^4 * A(x)).
 * https://oeis.org/A362393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (k+1)^(n-3*k-1) / (k! *
 *            (n-4*k)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"  /* seq_t pow_seq(seq_t a, int k) */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition; the formula gives 1 as well, but
       we avoid a negative exponent in pow_seq. */
    if (n == 0)
        return (seq_t)1;

    idx_t limit = n / 4;          /* floor(n/4) */
    seq_t sum = (seq_t)0;

    seq_t numerator   = (seq_t)1;  /* product of the 4k larger numbers */
    seq_t denominator = (seq_t)1;  /* k! */

    for (idx_t k = 0; k <= limit; ++k)
    {
        /* exponent = n - 3*k - 1  (non‑negative for k <= floor(n/4) ) */
        idx_t exp_idx = n - 3*k - 1;
        seq_t pow_term = pow_seq((seq_t)(k + 1), (int)exp_idx);

        /* comb = numerator / denominator  (exact division) */
        seq_t comb = numerator / denominator;

        /* term = comb * pow_term */
        seq_t term = comb * pow_term;
        sum += term;

        /* Prepare objects for the next k */
        if (k != limit)
        {
            seq_t m1 = (seq_t)(n - 4*k);         /* n-4k   */
            seq_t m2 = (seq_t)(n - 4*k - 1);     /* n-4k-1 */
            seq_t m3 = (seq_t)(n - 4*k - 2);     /* n-4k-2 */
            seq_t m4 = (seq_t)(n - 4*k - 3);     /* n-4k-3 */

            numerator   = numerator * m1 * m2 * m3 * m4;
            denominator = denominator * (seq_t)(k + 1);  /* multiply by (k+1) */
        }
    }

    return sum;
}
