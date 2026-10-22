/*
 * A308474  a(n) = Sum_{k=1..n^2, gcd(n,k) = 1} k.
 * https://oeis.org/A308474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * phi(n) / 2 for n > 1; a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 306, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    seq_t nn = (seq_t)n;
    seq_t phi_n = phi((idx_t)n);
    seq_t prod = nn * nn * nn;
    prod = prod * phi_n;
    prod /= 2;
    return prod;
}
