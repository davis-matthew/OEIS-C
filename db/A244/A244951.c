/*
 * A244951  Minimum number of colors needed to color the faces of the Platonic
 *          solids such that no two faces meeting at a common edge share the
 *          same color (in the order tetrahedron, cube, octahedron,
 *          dodecahedron, icosahedron).
 * https://oeis.org/A244951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 4 if n ≡ 1 (mod 3), 3 if n ≡ 2 (mod 3), 2 otherwise
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute n modulo 3; idx_t is unsigned, mod works as expected. */
    switch (n % 3) {
        case 1: return 4; /* tetrahedron, dodecahedron, …   */
        case 2: return 3; /* cube, icosahedron, …             */
        default:           /* n % 3 == 0, i.e., octahedron, …  */
            return 2;
    }
}
