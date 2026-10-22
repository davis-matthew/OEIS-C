/*
 * A362604  Expansion of e.g.f. 1/(1 + LambertW(-x * exp(x^2))).
 * https://oeis.org/A362604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2*k)^(n-k) / (k! *
 *            (n-2*k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* sum over k */
    seq_t sum = 0;
    /* falling factorial (n)_(2k), initialized for k=0 to 1 */
    seq_t falling = 1;
    /* k! , initialized for k=0 to 1 */
    seq_t fact_k  = 1;

    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* m = n-2k */
        idx_t m = n - 2 * k;

        /* compute power = (n-2k)^(n-k)  */
        seq_t power = big_pow((seq_t)m, (seq_t)(n - k));

        /* term = falling * power / fact_k  */
        seq_t term = falling * power / fact_k;
        sum += term;

        /* prepare falling & fact_k for next k */
        if (k + 1 <= n / 2)
        {
            /* multiply falling by (n-2k) * (n-2k-1) for next 2*(k+1) factors */
            seq_t factor1 = (seq_t)(n - 2 * k);
            seq_t factor2 = (seq_t)(n - 2 * k - 1);
            falling *= factor1;
            falling *= factor2;
            /* update factorial */
            fact_k *= (seq_t)(k + 1);
        }
    }

    return sum;
}
