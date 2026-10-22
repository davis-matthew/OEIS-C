/*
 * A400700  Start with a set of four adjacent collinear points in a hexagonal
 *          lattice. Every iteration, add to the set the points in the lattice
 *          which are adjacent to at least two points already in the set. This
 *          sequence gives the number of points in the set after n iterations.
 * https://oeis.org/A400700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = (3n^2 + 13n + 12 + 2*(n mod 3))/3
 * Range:     u8 n <= 7, u16 n <= 252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = ( (seq_t)3 * nn * nn
                  + (seq_t)13 * nn
                  + (seq_t)12
                  + (seq_t)2 * (seq_t)(n % 3) );
    return term / 3;
}
