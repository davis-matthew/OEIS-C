/*
 * A304511  a(n) = 318*2^n - 186 (n>=1).
 * https://oeis.org/A304511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 318*2^n - 186 (n>=1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"  /* for pow2(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* defined for n>=1 only, safety */
    seq_t val = (seq_t)318 * pow2(n);    /* 318 * 2^n in seq_t */
    return val - 186;
}
