/*
 * A357946  a(n) is the number in the infinite multiplication table that the
 *          chess knight reaches in n moves, starting from the number 1, the
 *          angle between adjacent segments being 90 degrees alternately
 *          changing direction to the left and to the right.
 * https://oeis.org/A357946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even? (3*n^2 + 8*n + 4)/4 : (3*n^2 + 16*n + 5)/4)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result;
    /* n is even */
    if ((n & 1) == 0)
        result = (3 * nn * nn + 8 * nn + 4) / 4;
    /* n is odd  */
    else
        result = (3 * nn * nn + 16 * nn + 5) / 4;
    return result;
}
