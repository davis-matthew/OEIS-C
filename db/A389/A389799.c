/*
 * A389799  Centered 30-gonal numbers.
 * https://oeis.org/A389799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^2 - 15*n + 1
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against the out-of-range offset; behaviour is undefined for n < 1 */
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    return 15 * (nn * (nn - 1)) + 1;
}
