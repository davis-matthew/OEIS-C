/*
 * A362701  Expansion of e.g.f. 1/(1 + LambertW(-x * exp(x^3/6))).
 * https://oeis.org/A362701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-3*k)^(n-2*k) / (6^k * k! *
 *            (n-3*k)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Sum over k from 0 to floor(n/3) */
    idx_t max_k = n / 3;

    seq_t sum   = 0;        /* running total */
    seq_t prod  = 1;        /* n!/(n-3k)! for current k  */
    seq_t pow6  = 1;        /* 6^k                       */
    seq_t factk = 1;        /* k!                        */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t m   = n - 3 * k;          /* n-3k   */
        idx_t exp = n - 2 * k;          /* n-2k   */

        /* (n-3k)^(n-2k) */
        seq_t pow_m = pow_seq((seq_t)m, (int)exp);

        /* term = prod * pow_m / (pow6 * factk) */
        seq_t term_num = prod * pow_m;
        seq_t term_den = pow6 * factk;
        seq_t term = term_num / term_den;

        sum += term;

        /* Prepare for next k */
        if (k < max_k)
        {
            /* prod_{k+1} = prod_k * m * (m-1) * (m-2)  */
            prod = prod * (seq_t)m * (seq_t)(m - 1) * (seq_t)(m - 2);
            pow6 *= 6;
            factk *= (k + 1);
        }
    }

    return sum;
}
