/*
 * A302058  Numbers that are not square pyramidal numbers.
 * https://oeis.org/A302058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + m if 6n > m(m-1)(2m+5) a(n) = n + m - 1 otherwise
 *            where m = ⌊(3n)^(1/3)⌋
 * Range:     u8 n <= 21, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t three_n = nn * 3;
    seq_t m_seq = floor_cube_root(three_n);
    idx_t m = (idx_t)m_seq;                  /* small enough for 64‑bit */

/* compute inequalities in arbitrary precision */
    seq_t lhs = nn * 6;

    seq_t tm = (seq_t)m;
    seq_t twom5 = tm * 2 + 5;
    seq_t rhs = tm * (tm - 1) * twom5;

/* choose result */
    if (lhs > rhs)
        return nn + (seq_t)m;
    else
        return nn + (seq_t)m - 1;
}
