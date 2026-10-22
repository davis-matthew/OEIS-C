/*
 * A338571  Number of polyhedra formed when the five Platonic solids, in the
 *          order tetrahedron, octahedron, cube, icosahedron, dodecahedron,
 *          are internally cut by all the planes defined by any three of their
 *          vertices.
 * https://oeis.org/A338571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=1, a(2)=8, a(3)=96, a(4)=2520, a(5)=552600 Number of
 *            polyhedra formed by cutting the five Platonic solids
 *            (tetrahedron, octahedron, cube, icosahedron, dodecahedron) with
 *            all planes defined by any three of their vertices. For n>5 the
 *            value is undefined
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t vals[] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)96,
        (seq_t)2520,
        (seq_t)552600
    };
    if (n >= 1 && n <= 5)
        return vals[n - 1];
    return (seq_t)0;
}
