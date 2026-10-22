/*
 * A332141  a(n) = 4*(10^(2*n+1)-1)/9 - 3*10^n.
 * https://oeis.org/A332141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1)-1)/9 - 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    // Compute 10^(2*n+1)
    idx_t exp = 2 * n + 1;
    seq_t ten_pow = pow10(exp);          // 10^(2*n+1)

    // Repunit of length (2*n+1): (10^(2*n+1)-1)/9
    seq_t rep = (ten_pow - 1) / 9;

    // a(n) = 4*rep - 3*10^n
    seq_t result = rep * 4;
    seq_t ten_pow_n = pow10(n);          // 10^n
    result -= ten_pow_n * 3;

    return result;
}
