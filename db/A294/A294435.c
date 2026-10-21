/*
 * A294435  a(n) = Sum_{m=0..n} (Sum_{k=0..m} binomial(n,k))^4.
 * https://oeis.org/A294435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} ( Sum_{k=0..m} binomial(n,k) )^4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for completeness, but not strictly required */

seq_t a(idx_t n)
{
    seq_t result = 0;            /* the sum over m */
    seq_t binom = 1;             /* binomial(n,0) = 1 */
    seq_t prefix = 0;            /* cumulative sum up to current m */

    for (idx_t m = 0; m <= n; ++m) {
        if (m > 0) {
            /* binom = binom * (n - m + 1) / m  (exact integer arithmetic) */
            seq_t numerator   = binom * (seq_t)(n - m + 1);
            binom = numerator / (seq_t)m;
        }
        /* prefix = sum_{k=0..m} binomial(n,k) */
        prefix += binom;

        /* compute prefix^4 = (prefix^2)^2 */
        seq_t sq = prefix * prefix;
        seq_t p4 = sq * sq;

        result += p4;
    }

    return result;
}
