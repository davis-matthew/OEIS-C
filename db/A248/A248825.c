/*
 * A248825  a(n) = n^2 + 1 - (-1)^n.
 * https://oeis.org/A248825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 1 - (-1)^n
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if ((n & 1) == 0)
        return tn * tn;
    else
        return tn * tn + 2;
}
