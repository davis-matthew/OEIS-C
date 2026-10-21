/*
 * A260679  a(n) = n + (17 - n)^2.
 * https://oeis.org/A260679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (17 - n)^2
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t diff;
    if (n >= 17) {
        diff = (seq_t)(n - 17);
    } else {
        diff = (seq_t)(17 - n);
    }
    seq_t sq = diff * diff;
    return (seq_t)n + sq;
}
