/*
 * A400538  Number of lattice points of the truncated tetrahedron whose
 *          vertices are the permutations of (+-n, +-n, +-3n) with an even
 *          number of minus signs.
 * https://oeis.org/A400538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 411, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A400538: Number of lattice points of the truncated tetrahedron whose vertices are the permutations of (+-n, +-n, +-3n) with an even number of minus signs.
 *
 * Formula: a(n) = (184*n^3 + 84*n^2 + 26*n + 3) / 3
 */
seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t term = ((seq_t)184 * nn * nn * nn
                 + (seq_t)84  * nn * nn
                 + (seq_t)26  * nn
                 + (seq_t)3) / (seq_t)3;
    return term;
}
