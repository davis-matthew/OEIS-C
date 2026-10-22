/*
 * A381009  Ordered areas of the Pythagorean triangles defined by a = 2^(4n) +
 *          2^(2n+1), b = 2^(4n) - 2^(4n-2) - 2^(2n) - 1, c = 2^(4n) +
 *          2^(4n-2) + 2^(2n) + 1.
 * https://oeis.org/A381009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(4n) + 2^(2n+1)) * (2^(4n) - 2^(4n-2) - 2^(2n) - 1) /
 *            2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Exponents used in the formula */
    idx_t exp4   = 4 * n;           /* 4*n   */
    idx_t exp2p1 = 2 * n + 1;       /* 2*n+1 */
    idx_t exp4_2 = 4 * n - 2;       /* 4*n-2 */
    idx_t exp2_0 = 2 * n;           /* 2*n   */

    /* 2^k values */
    seq_t p4    = pow2(exp4);
    seq_t p2p1  = pow2(exp2p1);
    seq_t p4_2  = pow2(exp4_2);
    seq_t p2_0  = pow2(exp2_0);

    /* Compute the product ( (2^(4n)+2^(2n+1)) * (2^(4n)-2^(4n-2)-2^(2n)-1) )/2 */
    seq_t first  = p4 + p2p1;          /* (2^(4n) + 2^(2n+1)) */
    seq_t second = p4 - p4_2;          /* 2^(4n) - 2^(4n-2)   */
    second -= p2_0;                    /* ... - 2^(2n)        */
    second -= (seq_t)1;                /* ... - 1             */

    seq_t product   = first * second;
    return product / (seq_t)2;
}
