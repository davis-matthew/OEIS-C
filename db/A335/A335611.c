/*
 * A335611  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(6,n)
 *          (with n at least 2) missing one edge.
 * https://oeis.org/A335611
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 31*63^(n-1) - 106*31^(n-1) + 145*15^(n-1) - 100*7^(n-1) +
 *            35*3^(n-1) - 5
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;             /* outside the offset */
    idx_t k = n - 1;                  /* exponent n-1 */

    seq_t p63 = pow_int((seq_t)63, k);
    seq_t p31 = pow_int((seq_t)31, k);
    seq_t p15 = pow_int((seq_t)15, k);
    seq_t p7  = pow_int((seq_t)7, k);
    seq_t p3  = pow_int((seq_t)3, k);

    /* Separate positive and negative parts to avoid unsigned underflow */
    seq_t pos = (seq_t)31 * p63 + (seq_t)145 * p15 + (seq_t)35 * p3;
    seq_t neg = (seq_t)106 * p31 + (seq_t)100 * p7 + (seq_t)5;

    return pos >= neg ? pos - neg : neg - pos;  /* pos is always >= neg here */
}
