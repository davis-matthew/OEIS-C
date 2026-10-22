/*
 * A338622  Irregular table read by rows: The number of k-faced polyhedra,
 *          where k>=4, formed when the five Platonic solids, in the order
 *          tetrahedron, octahedron, cube, icosahedron, dodecahedron, are
 *          internally cut by all the planes defined by any three of their
 *          vertices.
 * https://oeis.org/A338622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   a(n) is the number of k‑faced polyhedra obtained when each *
 *            Platonic solid (tetrahedron, octahedron, cube, icosahedron,
 *            dodecahedron) * is cut by all planes defined by any three of its
 *            vertices. The terms * are listed row‑wise as described in the
 *            OEIS entry for A338622. The * sequence has only 1...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _a_values[13] = {
    /* tetrahedron */ 1,
    /* octahedron */ 8,
    /* cube       */ 72, 24,
    /* icosahedron */ 2160, 360,
    /* dodecahedron */ 205320, 208680, 94800,
                       34200, 7920, 1560, 120
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at offset 1 */
    if (n <= 13) return _a_values[n - 1];
    return 0;
}
