/*
 * A394724  a(n) = (7*n)!/((4*n)!*(n!)^3).
 * https://oeis.org/A394724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n,3*n) * binomial(3*n,n) * binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t seven_n = n * 7;
    idx_t three_n = n * 3;
    idx_t two_n   = n * 2;

    seq_t res = binomial(seven_n, three_n);
    res = res * binomial(three_n, n);
    res = res * binomial(two_n, n);

    return res;
}
