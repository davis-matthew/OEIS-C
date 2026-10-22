/*
 * A322556  The number of eigenvectors with eigenvalue 1 summed over all
 *          linear operators on the vector space GF(2)^n.
 * https://oeis.org/A322556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) * 2^(n^2 - n) (= 2^(n^2) - 2^(n^2 - n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t sq = n * n;            /* n^2 fits in idx_t for n <= 10000 */
    seq_t p1 = pow2(sq);         /* 2^(n^2)   */
    seq_t p2 = pow2(sq - n);     /* 2^(n^2-n) */
    return p1 - p2;              /* difference is the desired value */
}
