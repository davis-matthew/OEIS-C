/*
 * A292543  Number of 5-cycles in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(n)=6*4^n for n>1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n≥2, a(n)=4*a(n-1) with a(2)=96 */
seq_t a(idx_t n)
{
    /* Offset 1: first term is a(1)=0 */
    if (n <= 1) {
        return 0;
    }

    seq_t result = 96;      /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        result *= (seq_t)4; /* multiply by 4 each step */
    }
    return result;
}
