/*
 * A378937  Number of minimal edge cuts in the 2 X n rook graph.
 * https://oeis.org/A378937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n-1) - 3^n + 5*2^(n-1) - 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset, return 0 */

    idx_t e1 = n * 2 - 1;          /* exponent for 2^(2n-1) */
    idx_t e2 = n - 1;              /* exponent for 2^(n-1) */

    seq_t part1 = pow2(e1);        /* 2^(2n-1) */
    seq_t part2 = pow3(n);         /* 3^n      */
    seq_t part3 = pow2(e2);        /* 2^(n-1)  */

    seq_t result = part1 - part2 + 5 * part3 - 3;
    return result;
}
