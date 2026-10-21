/*
 * A285956  Number of orthogonal rectangles with vertices on an n X n square
 *          grid of points but with no vertices on the grid's diagonals.
 * https://oeis.org/A285956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A285956: Number of orthogonal rectangles with vertices on an n × n square grid
   but with no vertices on the grid's diagonals.
   Formula (conjectured by Colin Barker, 2017):
   a(n) = (n⁴ - 10 n³ + 33 n² - 34 n)/4          for n even
   a(n) = (n⁴ - 10 n³ + 37 n² - 58 n + 30)/4     for n odd
   The formula is valid for all n ≥ 0. */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;       /* n         */
    seq_t n2  = nn * nn;        /* n²        */
    seq_t n3  = n2 * nn;        /* n³        */
    seq_t n4  = n3 * nn;        /* n⁴        */

    if ((n & 1) == 0) { /* n even */
        return (n4 - (seq_t)10 * n3 + (seq_t)33 * n2 - (seq_t)34 * nn) / 4;
    } else {            /* n odd  */
        return (n4 - (seq_t)10 * n3 + (seq_t)37 * n2 - (seq_t)58 * nn + (seq_t)30) / 4;
    }
}
