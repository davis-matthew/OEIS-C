/*
 * A389325  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *          binomial(n+k-1,n-2*k).
 * https://oeis.org/A389325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k) * binomial(n+k-1,n-2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(n, k);
        idx_t n2 = n + k - 1;
        idx_t r  = n - 2 * k;
        seq_t c2 = binomial(n2, r);
        sum += c1 * c2;
    }
    return sum;
}
