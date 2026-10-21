/*
 * A298360  Numbers congruent to {3, 7, 13, 27} mod 30.
 * https://oeis.org/A298360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/4)*30 + {3,7,13,27}[(n-1)%4]
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n == 0 the sequence is undefined; return 0 for safety */
    if (n == 0) return 0;

    static const seq_t residues[4] = { (seq_t)3, (seq_t)7, (seq_t)13, (seq_t)27 };

    idx_t k = n - 1;
    idx_t g = k / 4;           /* number of full 30‑step blocks before this term   */
    idx_t r = k % 4;           /* which residue inside the block                 */

    return ((seq_t)g) * (seq_t)30 + residues[r];
}
