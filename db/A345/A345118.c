/*
 * A345118  a(n) is the sum of the lengths of all the segments used to draw a
 *          square of side n representing a basketweave pattern where all the
 *          multiple strands are of unit width, the horizontal ones appearing
 *          as 1 X 3 rectangles, while the vertical ones as unit area squares.
 * https://oeis.org/A345118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5+3*n)/2 if n even a(n) = n*(5+3*n)/2 if n mod 8 == 1
 *            or 5 a(n) = (n*(5+3*n)-2)/2 if n mod 8 == 3 a(n) =
 *            (n*(5+3*n)+2)/2 if n mod 8 == 7 (also works for n=0)
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t base = nn * ((seq_t)3 * nn + (seq_t)5);       /* n*(5+3*n) */
    if ((n & (idx_t)1) != 0) {                         /* odd n */
        idx_t r = n % (idx_t)8;
        if (r == (idx_t)3)
            base -= (seq_t)2;
        else if (r == (idx_t)7)
            base += (seq_t)2;
    }
    return base / (seq_t)2;
}
