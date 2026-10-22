/*
 * A387933  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(4*n,k).
 * https://oeis.org/A387933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(4*n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t four_n = 4 * n;          /* compute 4*n once, safe for uint64_t */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow3(n - k) * binomial(n, k) * binomial(four_n, k);
        sum += term;
    }
    return sum;
}
