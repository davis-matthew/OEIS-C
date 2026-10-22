/*
 * A332115  a(n) = (10^(2*n+1) - 1)/9 + 4*10^n.
 * https://oeis.org/A332115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1) - 1)/9 + 4*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t e = 2 * n + 1;
    seq_t rep = pow10(e);
    rep -= 1;
    rep /= 9;
    seq_t add = pow10(n);
    add *= 4;
    return rep + add;
}
