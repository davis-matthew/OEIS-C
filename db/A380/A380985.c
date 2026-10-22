/*
 * A380985  Numbers whose k-th digit indicates the number of digits which
 *          occur k times.
 * https://oeis.org/A380985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 (n=1), 20 (n=2), 110 (n=3), 2*10^(n-1)+100 for n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)110;
    seq_t v = pow10(n - 1);          // v = 10^(n-1)
    v = v * (seq_t)2 + (seq_t)100;   // v = 2*10^(n-1) + 100
    return v;
}
