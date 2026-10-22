/*
 * A337960  Number of achiral colorings of the 30 triangular faces of a
 *          regular icosahedron or the 30 vertices of a regular dodecahedron
 *          using n or fewer colors.
 * https://oeis.org/A337960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (15*n^10 + n^8 + 20*n^2 + 24) / 60
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          // n
    seq_t n2   = nn * nn;           // n^2
    seq_t n4   = n2 * n2;           // n^4
    seq_t n8   = n4 * n4;           // n^8
    seq_t n10  = n2 * n8;           // n^10
    seq_t term = (seq_t)15 * n10 + n8 + (seq_t)20 * n2 + (seq_t)24;
    seq_t numer = n2 * term;        // n^2 * (15*n^10 + n^8 + 20*n^2 + 24)
    seq_t result = numer / (seq_t)60;   // divide by 60
    return result;
}
