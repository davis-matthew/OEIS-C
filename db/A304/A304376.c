/*
 * A304376  a(n) = 60*2^n - 48 (n>=1).
 * https://oeis.org/A304376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*2^n - 48
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* behaviour below offset is undefined */

    seq_t val = pow2(n);    /* 2^n */
    val *= (seq_t)60;       /* 60*2^n */
    val -= (seq_t)48;       /* 60*2^n - 48 */

    return val;
}
