/*
 * A383577  a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,k) *
 *          binomial(n-2*k,k)^2.
 * https://oeis.org/A383577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-k,k) *
 *            binomial(n-2*k,k)^2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t kmax = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = binom(n - k, k);
        seq_t t2 = binom(n - 2 * k, k);
        sum += t1 * t2 * t2;
    }
    return sum;
}
