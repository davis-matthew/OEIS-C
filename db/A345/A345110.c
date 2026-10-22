/*
 * A345110  a(n) is n rotated one place to the left or, equivalently, n with
 *          the most significant digit moved to the least significant place,
 *          omitting leading zeros.
 * https://oeis.org/A345110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is n rotated one place to the left (most significant *
 *            decimal digit moved to the least significant place). For n = 0,
 *            a(0)=0
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 */

    int d = digits(n);                 /* number of decimal digits of n (>=1) */
    seq_t pw = pow10(d - 1);            /* 10^(d-1) */
    seq_t msd = (seq_t)n / pw;          /* most significant digit */
    seq_t rem = (seq_t)n % pw;          /* remaining digits */

    return rem * 10 + msd;             /* rotate left */
}
