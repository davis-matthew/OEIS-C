/*
 * A375256  Number of pairs of antipodal vertices in the level n Hanoi graph.
 * https://oeis.org/A375256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2^(2n-3)+3*2^(n-2)-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 3 */
    if (n == 1)
        return (seq_t)3;

    /* Exponents for 2^(2n-3) and 2^(n-2) */
    idx_t exp1 = (n << 1) - 3;   /* 2*n - 3 */
    idx_t exp2 = n - 2;          /* n - 2 */

    /* Compute the power-of-two terms */
    seq_t p1 = pow2(exp1);       /* 2^(2n-3) */
    seq_t p2 = pow2(exp2);       /* 2^(n-2) */

    /* Sum the components: 2^(2n-3) + 3*2^(n-2) - 1 */
    seq_t sum = p1 + (p2 * (seq_t)3) - (seq_t)1;

    /* Multiply the final sum by 3 */
    return sum * (seq_t)3;
}
