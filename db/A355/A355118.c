/*
 * A355118  The number of nonequivalent root ancestral configurations in a
 *          recursively defined family of gene trees and species trees with at
 *          least n = 9 leaves, in which for n >= 12 leaves, 3-leaf trees are
 *          successively added at the root of the tree with n-3 leaves.
 * https://oeis.org/A355118
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(⎣n/3⎦) * (2 · (n-3⎣n/3⎦)^2 + 8 · (n-3⎣n/3⎦) + 23) / 27
 *            For n ≥ 9 this equals 3^(⎣n/3⎦-3) · (2k^2 + 8k + 23) where k = n
 *            mod 3
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 61, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 9; return 0 as a placeholder. */
    if (n < 9)
        return 0;

    /* floor(n/3) and n mod 3 */
    idx_t r = n / 3;                /* ⎣n/3⎦          */
    idx_t k = n - 3 * r;            /* n mod 3 (0,1,2) */

    /* Numerator: 2k^2 + 8k + 23  (fits easily in seq_t). */
    seq_t num = (seq_t)(2 * k * k + 8 * k + 23);

    /* Compute 3^(r-3).  For n ≥ 9, r ≥ 3. */
    seq_t pow_r3 = seq_pow((seq_t)3, (idx_t)(r - 3));

    /* Final result: 3^(r-3) * (2k^2 + 8k + 23) */
    return seq_mul(pow_r3, num);
}
