/*
 * A383528  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k,k) * binomial(n-2*k,k).
 * https://oeis.org/A383528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+k,k) * binomial(n-2*k,k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term1 = binomial(n + k, k);
        seq_t term2 = binomial(n - 2 * k, k);
        sum += term1 * term2;
    }
    return sum;
}
