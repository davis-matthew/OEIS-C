/*
 * A367360  Comma transform of squares.
 * https://oeis.org/A367360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*last_digit(n^2) + first_digit((n+1)^2)
 * Range:     u8 n <= 14, u16 n <= 1637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to the integer type used for sequence values */
    seq_t tn = (seq_t)n;

    /* last digit of n^2 */
    seq_t sq = tn * tn;
    seq_t last = sq % (seq_t)10;

    /* first digit of (n+1)^2 */
    seq_t np1 = tn + (seq_t)1;
    seq_t sq_np1 = np1 * np1;
    seq_t first = sq_np1;
    while (first >= (seq_t)10)
        first /= (seq_t)10;

    /* assemble the two-digit result */
    return last * (seq_t)10 + first;
}
