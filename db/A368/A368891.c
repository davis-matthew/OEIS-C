/*
 * A368891  a(n) = Sum_{k=0..floor(n/3)} n^k * binomial(n-2*k,k).
 * https://oeis.org/A368891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} n^k * binomial(n-2*k,k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t pow = 1;          /* n^k */
    idx_t kmax = n / 3;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t bin = binomial(n - 2 * k, k);
        sum += pow * bin;
        pow *= (seq_t)n;
    }
    return sum;
}
