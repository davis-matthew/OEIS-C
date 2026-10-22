/*
 * A335956  a(n) = (2^n - 1)*2^valuation(n, 2) for n > 0 and a(0) = 0.
 * https://oeis.org/A335956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) * 2^valuation(n, 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute the 2-adic valuation of n (number of trailing zeros) */
    idx_t tmp = n;
    unsigned v = 0;
    while ((tmp & 1ULL) == 0) {
        ++v;
        tmp >>= 1;
    }

    /* 2^n - 1 */
    seq_t pow2_n = pow2(n);
    seq_t term = pow2_n - 1;

    /* 2^valuation(n, 2) */
    seq_t pow2_v = pow2(v);

    /* final product */
    return term * pow2_v;
}
