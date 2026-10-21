/*
 * A296602  Values of F for which there is a unique convex polyhedron with F
 *          faces that are all regular polygons.
 * https://oeis.org/A296602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 23 for n >= 14, a(n) given by table for n <= 14
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 14 terms */
    static const seq_t init[] = {
        4, 19, 23, 25, 29, 31, 33, 35, 39, 41,
        43, 45, 49, 51
    };

    if (n <= 14)
        return init[n - 1];
    else
        return (seq_t)n * 2 + 23;
}
