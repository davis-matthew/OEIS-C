/*
 * A391073  a(n) is the number of nondegenerate triangles (n, n + d, n + 2*d)
 *          with integer area for some nonnegative integers d.
 * https://oeis.org/A391073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | 1 ≤ d < n and the area of the triangle (n, n+d,
 *            n+2d) is an integer }
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t count_triangles(idx_t n)
{
    if (n < 2u) return 0;   /* no admissible d */

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t d = 1; d < n; ++d) {
        seq_t d_seq = (seq_t)d;

        /* sides: a = n,  b = n+d,  c = n+2d */
        /* build intermediate values for the Heron formula */
        seq_t t      = n_seq + d_seq;           /* n + d   */
        seq_t term1  = n_seq + 3 * d_seq;       /* n + 3d  */
        seq_t term2  = n_seq - d_seq;           /* n - d   */

        /* N = 3 * (n+d)^2 * (n+3d) * (n-d)  = 16 * area^2   */
        seq_t N = 3 * t * t * term1 * term2;

        /* area^2 must be integer, so N must be divisible by 16 */
        if ((N & 15u) != 0u)
            continue;

        seq_t area_sq = N >> 4;                 /* N / 16  */
        seq_t root    = floor_sqrt(area_sq);   /* floor(sqrt(area_sq)) */

        if (root * root == area_sq)
            ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    return count_triangles(n);
}
