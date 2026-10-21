/*
 * A261953  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable side of the triangles
 *          of the (n-1)-th generation (this is the "side to side" version);
 *          for the even n-th generation use the "vertex to vertex" version;
 *          a(n) is the number of triangles a...
 * https://oeis.org/A261953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1, if n is even: a(n) = 9*n/2, if n is odd :
 *            a(n) = (9*n - 3)/2 (derived from a(n) = (3*(-1+(-1)^n+6*n))/4)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0u)          /* even n */
        return (nn * (seq_t)9) / (seq_t)2;
    else                          /* odd n */
        return ((nn * (seq_t)9) - (seq_t)3) / (seq_t)2;
}
