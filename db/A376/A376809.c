/*
 * A376809  Expansion of 1/sqrt(1 - 4*x^3/(1 - x)^2).
 * https://oeis.org/A376809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) *
 *            binomial(n-k-1,n-3*k)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n / 3;                   /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(2 * k, k);
        seq_t c2 = binomial(n - k - 1, n - 3 * k);
        sum += c1 * c2;
    }
    return sum;
}
