/*
 * A330271  a(n) is the least nonnegative integer k such that n XOR k is a
 *          cube (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A330271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimum k ≥ 0 with (n XOR k) a perfect cube
 * Range:     u8 n <= 63, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine searches k from 0 upward; for each k it tests whether n ^ k
   equals m³ for some integer m.  Since k = n clearly gives cube 0,
   the minimal k is always ≤ n, so the search is bounded by k ≤ n. */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;               /* n in the sequence type */
    for (idx_t k = 0; k <= n; ++k) {  /* search increasing k */
        seq_t v      = N ^ (seq_t)k;   /* candidate cube candidate */
        seq_t root   = floor_cube_root(v);
        seq_t cube   = root * root * root; /* might overflow but compare */
        if (cube == v) {
            return (seq_t)k;
        }
    }
    /* This point is unreachable because k = n always works (cube = 0). */
    return N;
}
