/*
 * A294689  Collatz cycle of negative numbers starting with -17.
 * https://oeis.org/A294689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = -17; a(n+1) = (3*a(n)+1)/2 if a(n) is odd, a(n)/2
 *            otherwise
 * Range:     i8 n <= 8, i16 n <= 44, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = -((seq_t)17);
    if (n <= 1) return cur;
    for (idx_t i = 1; i < n; ++i)
    {
        if ((cur & 1) != 0)
            cur = (3 * cur + 1) / 2;
        else
            cur = cur / 2;
    }
    return cur;
}
