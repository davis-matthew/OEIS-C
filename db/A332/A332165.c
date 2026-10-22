/*
 * A332165  a(n) = 6*(10^(2*n+1)-1)/9 - 10^n.
 * https://oeis.org/A332165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6 * (10^(2*n+1) - 1) / 9 - 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow2n1 = pow10(2 * n + 1);   /* 10^(2*n+1)   */
    seq_t pown   = pow10(n);           /* 10^n         */

    seq_t part = pow2n1 - (seq_t)1;    /* 10^(2*n+1) - 1 */
    seq_t part_div9 = part / 9;        /* (10^(2*n+1) - 1)/9 */
    seq_t part_mul6 = part_div9 * 6;   /* *6 */

    return part_mul6 - pown;           /* subtract 10^n */
}
