/*
 * A362085  a(n) = Sum_{k=0..n} (-1)^k * binomial(-3,k) * binomial(n*k,n-k).
 * https://oeis.org/A362085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,k) * binomial(n*k,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t b1 = binomial(k + 2, k);
        idx_t nk = n * k;
        idx_t nk_minus = n - k;
        seq_t b2 = binomial(nk, nk_minus);
        sum += seq_mul(b1, b2);
    }
    return sum;
}
