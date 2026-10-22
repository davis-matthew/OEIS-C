/*
 * A366192  Pairs (i, j) of noncoprime positive integers sorted first by i + j
 *          then by i.
 * https://oeis.org/A366192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 9886, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate non‑coprime pairs (i,j) sorted by i+j and i;
   output the first, then the second coordinate of each pair. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t s = 4; ; ++s) {               /* i + j starts at 4 (2+2) */
        for (idx_t i = 2; i <= s - 2; ++i) {  /* i ≥ 2, j = s-i ≥ 2 */
            idx_t j = s - i;
            if (gcd((seq_t)i, (seq_t)j) > 1) {
                ++count;
                if (count == n) return (seq_t)i;
                ++count;
                if (count == n) return (seq_t)j;
            }
        }
    }
}
