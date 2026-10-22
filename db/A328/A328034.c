/*
 * A328034  a(n) = 3n minus the largest power of 2 not exceeding 3n.
 * https://oeis.org/A328034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - 2^floor(log2(3*n))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* compute 3*n using seq_t to avoid overflow */
    seq_t m = (seq_t)n * 3;
    /* floor log2(m) is the bit length minus one */
    unsigned bl = bitlen_seq_t(m);
    /* largest power of 2 ≤ m */
    seq_t p = ((seq_t)1) << (bl - 1);
    return m - p;
}
