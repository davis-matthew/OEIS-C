/*
 * A386670  Number of ternary strings of length 2*n that have more 0's than
 *          the combined number of 1's and 2's.
 * https://oeis.org/A386670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^(n-k)*binomial(2*n,n-k) This is equivalent
 *            to a(n) = Sum_{t=0}^{n-1} 2^t * binomial(2*n, t) which is
 *            computed in O(n) time using multiplicative updating of the
 *            binomial coefficients
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the base case immediately. */
    if (n == 0) return 0;
    idx_t two_n = n + n;
    seq_t comb  = 1;   /* C(2n,0) */
    seq_t power = 1;   /* 2^0   */
    seq_t sum   = 0;
    for (idx_t t = 0; t < n; ++t) {
        /* Add the current term 2^t * C(2n, t) to the sum. */
        sum += power * comb;
        /* Update 2^(t+1). */
        power = power * 2;
        /* Update the binomial coefficient for the next t+1. */
        if (t + 1 < n) {
            comb = comb * (two_n - t);
            comb = comb / (t + 1);
        }
    }
    return sum;
}
