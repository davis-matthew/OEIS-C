/*
 * A304605  a(n) = 48*2^n + 26 (n>=1).
 * https://oeis.org/A304605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*2^n + 26
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Calls with n < 1 are ignored. */
    seq_t val = pow2(n);          /* 2^n */
    val *= (seq_t)48;             /* 48 * 2^n */
    val += (seq_t)26;             /* +26 */
    return val;
}
