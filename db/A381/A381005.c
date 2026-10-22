/*
 * A381005  Ordered short legs of the Pythagorean triangles defined by a =
 *          2^(4n) + 2^(2n+1), b = 2^(4n) - 2^(4n-2) - 2^(2n) - 1, c = 2^(4n)
 *          + 2^(4n-2) + 2^(2n) + 1.
 * https://oeis.org/A381005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n) - 2^(4n-2) - 2^(2n) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior below offset does not matter */
        return 0;

    /* first three terms computed directly from the closed form */
    seq_t a1 = pow2(4)  - pow2(2) - pow2(2)  - 1;  /* a(1) */
    seq_t a2 = pow2(8)  - pow2(6) - pow2(4)  - 1;  /* a(2) */
    seq_t a3 = pow2(12) - pow2(10)- pow2(6)  - 1;  /* a(3) */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)21 * a3 - (seq_t)84 * a2 + (seq_t)64 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
