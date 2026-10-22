/*
 * A381006  Ordered long legs of the Pythagorean triangles defined by a =
 *          2^(4n) + 2^(2n+1), b = 2^(4n) - 2^(4n-2) - 2^(2n) - 1, c = 2^(4n)
 *          + 2^(4n-2) + 2^(2n) + 1.
 * https://oeis.org/A381006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n) + 2^(2n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term */
    if (n < 1)
        return 0;              /* undefined for n<1, return 0 */

    /* compute exponents as idx_t to avoid overflow of int types */
    idx_t exp1 = 4 * n;
    idx_t exp2 = 2 * n + 1;

    seq_t term1 = pow2(exp1); /* 2^(4n) */
    seq_t term2 = pow2(exp2); /* 2^(2n+1) */

    return term1 + term2;
}
