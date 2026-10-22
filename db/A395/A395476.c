/*
 * A395476  Expansion of e.g.f. x^3*(1 + x + x^2/2)*exp(x).
 * https://oeis.org/A395476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^2-5*n+8)/2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 98, u64 n <= 8193,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    unsigned __int128 nn = n;
    unsigned __int128 prod = nn * (nn - 1) * (nn - 2);
    unsigned __int128 d = nn * nn - (unsigned __int128)5 * nn + 8;
    prod *= d;
    prod /= 2;

    return (seq_t)prod;
}
