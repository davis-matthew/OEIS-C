/*
 * A242737  Number of stellations of Platonic solids when applying Miller's
 *          rules (in the order tetrahedron, cube, octahedron, dodecahedron,
 *          icosahedron).
 * https://oeis.org/A242737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242737: Number of Miller stellations of Platonic solids
   In the order tetrahedron, cube, octahedron, dodecahedron, icosahedron.
   The values are known constants for n = 1..5.  For any other index
   a(n) is defined to be 0. */
seq_t a(idx_t n)
{
    static const seq_t table[5] = {
        (seq_t)1,  /* tetrahedron   */
        (seq_t)1,  /* cube          */
        (seq_t)2,  /* octahedron    */
        (seq_t)4,  /* dodecahedron  */
        (seq_t)59  /* icosahedron   */
    };
    if (n >= 1 && n <= 5)
        return table[n-1];
    return (seq_t)0;
}
