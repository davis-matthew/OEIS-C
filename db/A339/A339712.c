/*
 * A339712  a(n) = Sum_{d|n} d^(d+n/d-1) * binomial(d+n/d-2, d-1).
 * https://oeis.org/A339712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} d^(d+n/d-1) * binomial(d+n/d-2, d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    /* iterate over all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t nd = n / d;
            idx_t exp_d = d + nd - 1;
            seq_t term_d = big_pow((seq_t)d, (seq_t)exp_d);
            term_d *= binomial((idx_t)(d + nd - 2), (idx_t)(d - 1));
            total += term_d;

            /* the paired divisor n/d, if distinct */
            if (nd != d) {
                idx_t exp_nd = nd + d - 1;   /* nd + n/nd - 1 == nd + d - 1 */
                seq_t term_nd = big_pow((seq_t)nd, (seq_t)exp_nd);
                term_nd *= binomial((idx_t)(nd + d - 2), (idx_t)(nd - 1));
                total += term_nd;
            }
        }
    }
    return total;
}
