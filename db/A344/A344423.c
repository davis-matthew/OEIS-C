/*
 * A344423  a(n) = 10^(2*n+2) + 111*10^n + 1.
 * https://oeis.org/A344423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(2*n+2) + 111*10^n + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 10^(2*n+2) (the latter directly) */
    seq_t ten_pow_n = pow10(n);
    seq_t ten_pow_2n2 = pow10(2 * n + 2);

    /* a(n) = 10^(2*n+2) + 111*10^n + 1 */
    return ten_pow_2n2 + mul_small(ten_pow_n, 111) + 1;
}
