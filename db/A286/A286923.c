/*
 * A286923  Positions of 0 in A286922; complement of A286924.
 * https://oeis.org/A286923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3+sqrt(2))*n - 1) = floor(sqrt(2)*n) + 3*n - 1. *
 *            The implementation uses integer arithmetic only. floor_sqrt()
 *            (from * "../utils/floor_sqrt.h") returns ⌊√x⌋ for a seq_t
 *            argument. We compute * ⌊√(2*n²)⌋, which equals ⌊n*√2⌋, then add
 *            3*n−1
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* convert index to seq_t */
    seq_t sq = 2 * nn * nn;     /* 2 * n^2 */
    seq_t fs = floor_sqrt(sq);  /* ⌊√(2*n²)⌋ = ⌊n*√2⌋ */
    return fs + 3 * nn - 1;     /* ⌊√2*n⌋ + 3*n - 1 */
}
