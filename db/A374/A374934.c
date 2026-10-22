/*
 * A374934  Maximum number of squares covered (i.e., attacked) by 4
 *          independent (i.e., nonattacking) queens on an n X n chessboard.
 * https://oeis.org/A374934
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 for n ≤ 7 * a(8) = 62 * a(9) = 76 * a(10)= 92 * a(n)
 *            = 16*n - 72 for n ≥ 11
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
    {
        return (seq_t)n * (seq_t)n;
    }
    else if (n == 8)
    {
        return (seq_t)62;
    }
    else if (n == 9)
    {
        return (seq_t)76;
    }
    else if (n == 10)
    {
        return (seq_t)92;
    }
    else
    {
        return (seq_t)16 * (seq_t)n - (seq_t)72;
    }
}
