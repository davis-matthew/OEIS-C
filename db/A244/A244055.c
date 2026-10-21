/*
 * A244055  Number of edges on each face of the Platonic solids (in the order
 *          tetrahedron, cube, octahedron, dodecahedron, icosahedron).
 * https://oeis.org/A244055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 3,4,3,5,3 repeated for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=3, a(2)=4, a(3)=3, a(4)=5, a(5)=3 */
seq_t a(idx_t n)
{
    static const seq_t vals[5] = { (seq_t)3,
                                  (seq_t)4,
                                  (seq_t)3,
                                  (seq_t)5,
                                  (seq_t)3 };
    idx_t idx = (n - 1) % 5;
    return vals[idx];
}
