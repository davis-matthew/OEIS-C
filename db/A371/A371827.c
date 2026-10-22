/*
 * A371827  a(n) = Sum_{k=0..floor(n/3)} n^k * binomial(2*n-2*k,n-3*k).
 * https://oeis.org/A371827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} n^k * binomial(2*n-2*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t power = 1;           /* n^k, initially n^0 = 1   */
    idx_t maxk  = n / 3;       /* floor(n/3)                */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t t = 2 * n - 2 * k;        /* t = 2*n - 2*k           */
        idx_t r = n - 3 * k;           /* r = n - 3*k             */
        seq_t bin = binomial(t, r);    /* binomial(t, r)          */
        sum += power * bin;            /* term contribution        */
        if (k < maxk)                  /* prepare n^{k+1}         */
            power *= (seq_t)n;
    }
    return sum;
}
