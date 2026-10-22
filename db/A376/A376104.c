/*
 * A376104  Expansion of e.g.f. -LambertW(-2*x / (1 + x))/2.
 * https://oeis.org/A376104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} (-1)^(n-k) * (2*k)^(k-1) *
 *            binomial(n-1,k-1) / k!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A376104: Expansion of e.g.f. -LambertW(-2*x/(1+x))/2.
   Offset: 0
*/

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* n! once for all terms */
    seq_t fact_n = factorial(n);

    seq_t ratio = fact_n;           /* n! / k!  (for current k) */
    seq_t binom = 1;                /* binomial(n-1, k-1)  */
    seq_t pos_sum = 0;              /* sum of terms with positive sign */
    seq_t neg_sum = 0;              /* sum of terms with negative sign */

    for (idx_t k = 1; k <= n; ++k) {
        /* term = (n! / k!) * binomial(n-1,k-1) * (2*k)^(k-1)  */
        seq_t pow_term = pow_uint((uint64_t)(2 * k), k - 1);
        seq_t term = ratio * binom;
        term *= pow_term;

        /* check sign: (-1)^(n-k) */
        if (((n - k) & 1) == 0) {
            pos_sum += term;
        } else {
            neg_sum += term;
        }

        /* prepare for next k (if any) */
        if (k < n) {
            /* ratio := n! / (k+1)!  =  (n! / k!) / (k+1) */
            ratio /= (k + 1);

            /* binom := binomial(n-1, k) = binom * (n - k) / k */
            binom = (binom * (n - k)) / k;
        }
    }

    /* final result = pos_sum - neg_sum (pos_sum >= neg_sum) */
    return pos_sum - neg_sum;
}
