/*
 * A332189  a(n) = 8*(10^(2n+1)-1)/9 + 10^n.
 * https://oeis.org/A332189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2*n+1)-1)/9 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute the repunit of length 2*n+1: 111…1 (2*n+1 digits) */
    seq_t rep = 0;
    idx_t len = 2 * n + 1;
    for (idx_t i = 0; i < len; ++i) {
        rep = rep * (seq_t)10 + (seq_t)1;
    }

    /* multiply by 8 and add 10^n */
    seq_t result = rep * (seq_t)8;
    seq_t p10 = pow_uint(10, n);
    result = result + p10;
    return result;
}
