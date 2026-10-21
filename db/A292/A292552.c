/*
 * A292552  Nontotients of the form 10^k - 2.
 * https://oeis.org/A292552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^(n+1) - 2 (a(1) = 98, a(2) = 998, …)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n < 1 are undefined. */
    if (n == 0) return 0;
    seq_t v = pow10(n + 1);
    return v - (seq_t)2;
}
