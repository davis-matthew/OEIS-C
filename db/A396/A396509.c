/*
 * A396509  a(n) = 2 * (4*n+2)^(n-1).
 * https://oeis.org/A396509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (4*n+2)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t base = (seq_t)n;
    base *= 4;          /* base = 4*n   */
    base += (seq_t)2;   /* base = 4*n+2 */

    /* Compute base^(n-1) and multiply by 2 */
    seq_t power = pow_seq(base, (int)(n - 1));
    return power * 2;
}
