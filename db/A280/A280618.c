/*
 * A280618  Expansion of (Sum_{k>=1} x^(k^3))^2.
 * https://oeis.org/A280618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of ordered pairs of positive cubes summing to n (i^3 +
 *            j^3 = n, with i, j ≥ 1, order matters)
 * Range:     u8 n <= 64, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   floor_cube_root, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* smallest sum 1^3+1^3 = 2 */

    seq_t result = 0;
    seq_t n_seq   = (seq_t)n;
    seq_t max_i   = floor_cube_root(n_seq - 1); /* i^3 ≤ n‑1 */

    for (idx_t ii = 1; ii <= (idx_t)max_i; ++ii) {
        seq_t i   = (seq_t)ii;
        seq_t i3  = pow_seq(i, 3);
        seq_t rem = n_seq - i3;
        if (rem == 0) continue;   /* j would be 0, not allowed */
        seq_t j = floor_cube_root(rem);
        if (pow_seq(j, 3) == rem) {
            ++result;
        }
    }
    return result;
}
