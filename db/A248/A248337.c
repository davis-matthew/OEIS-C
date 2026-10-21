/*
 * A248337  a(n) = 6^n - 4^n.
 * https://oeis.org/A248337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 4^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow6 = pow_int((seq_t)6, n);
    seq_t pow4 = pow_int((seq_t)4, n);
    return pow6 - pow4;
}
