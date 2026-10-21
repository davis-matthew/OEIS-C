/*
 * A267982  a(n) = 4*n*Catalan(n)^2.
 * https://oeis.org/A267982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*Catalan(n)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t res = 4;
    res *= (seq_t)n;
    res *= cat;
    res *= cat;
    return res;
}
