/*
 * A371819  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-k+1,n-3*k).
 * https://oeis.org/A371819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binom(2*n - k + 1, n -
 *            3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t pos = 0, neg = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t a_top = 2 * n - k + 1;
        idx_t a_bot = n - 3 * k;
        seq_t term = binom(a_top, a_bot);

        if ((k & 1) == 0)
            pos += term;
        else
            neg += term;
    }

    return pos - neg;   /* sequence values are known to be positive, so subtract safely */
}
