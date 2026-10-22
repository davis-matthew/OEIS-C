/*
 * A389125  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(2*k,n-3*k).
 * https://oeis.org/A389125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(2*k,
 *            n-3*k)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 51, u128 n <= 99,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n, k);
        seq_t t2 = binomial(2 * k, n - 3 * k);
        sum += t1 * t2;
    }
    return sum;
}
