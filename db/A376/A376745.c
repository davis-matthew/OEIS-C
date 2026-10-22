/*
 * A376745  Numbers that are not pentagonal pyramidal numbers.
 * https://oeis.org/A376745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+m if 2n > m(m−1)(m+2) * a(n) = n+m−1 else * where m =
 *            floor((2n)^(1/3))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* According to OEIS definition, n ≥ 1.
       If n == 0 the result is undefined, so return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Two times the index, computed in idx_t to avoid overflow in
       very small seq_t types. */
    idx_t two_n = n << 1;               /* 2 * n */
    idx_t m_idx = int_cube_root(two_n); /* floor cube root of 2n */
    seq_t m = (seq_t)m_idx;

    /* Compute m*(m-1)*(m+2) in seq_t, intermediate values are small
       (m ≤ cube root of 2n). */
    seq_t left = m * (m - 1);
    left *= (m + 2);

    seq_t result;
    if (two_n > left)
        result = (seq_t)n + m;
    else
        result = (seq_t)n + m - 1;

    return result;
}
