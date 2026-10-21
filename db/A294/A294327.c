/*
 * A294327  a(n) = ((9*n + 8)*10^n - 8)/9.
 * https://oeis.org/A294327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9*n + 8)*10^n - 8)/9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                 /* cast index to seq_t */
    seq_t nine_n_plus_8 = tn * 9 + 8;    /* 9*n + 8            */
    seq_t ten_n = pow10(n);              /* 10^n                */
    seq_t numerator = nine_n_plus_8 * ten_n - 8; /* (9*n+8)*10^n - 8 */
    seq_t result = numerator / 9;        /* divide by 9         */
    return result;
}
