/*
 * A274638  Main diagonal of A274637.
 * https://oeis.org/A274638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 1 (n even) a(n) = 2^n - 1 - 2^((n-1)/2) (n odd)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n = pow2(n);
    seq_t res = two_n - 1;
    if (n & 1)
    {
        seq_t two_half = pow2((n - 1) / 2);
        res -= two_half;
    }
    return res;
}
