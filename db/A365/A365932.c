/*
 * A365932  a(n) = the number of cubes (of integers > 0) that have bit length
 *          n.
 * https://oeis.org/A365932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor((2^n-1)^(1/3)) - floor((2^(n-1)-1)^(1/3))
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_cube_root, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow2n   = pow2(n);
    seq_t pow2nm1 = pow2(n - 1);
    pow2n  -= (seq_t)1;
    pow2nm1 -= (seq_t)1;
    seq_t cr_n   = floor_cube_root(pow2n);
    seq_t cr_nm1 = floor_cube_root(pow2nm1);
    return cr_n - cr_nm1;
}
