/*
 * A245179  Numbers of the form 2^k+3 or 3*2^k+1, k >= 2.
 * https://oeis.org/A245179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2^(k+2)+3, a(2k+1) = 3*2^(k+1)+1
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if (n & 1) {
        /* odd index: a(2k+1) */
        idx_t k = n >> 1;          /* k = floor(n/2) */
        seq_t val = pow2(k + 1);   /* 2^(k+1) */
        val = val * (seq_t)3;      /* 3 * 2^(k+1) */
        val += (seq_t)1;           /* + 1 */
        return val;
    } else {
        /* even index: a(2k) */
        idx_t k = n >> 1;          /* k = n/2 */
        seq_t val = pow2(k + 2);   /* 2^(k+2) */
        val += (seq_t)3;           /* + 3 */
        return val;
    }
}
