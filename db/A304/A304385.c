/*
 * A304385  a(n) = 192*2^n - 31 (n>=1).
 * https://oeis.org/A304385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 192*2^n - 31 (n >= 1)
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n as seq_t */
    seq_t val = pow2(n);

    /* 192 * 2^n - 31 */
    val = val * (seq_t)192;
    val = val - (seq_t)31;

    return val;
}
