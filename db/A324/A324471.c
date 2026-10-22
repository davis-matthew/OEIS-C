/*
 * A324471  a(n) = 10 mod n.
 * https://oeis.org/A324471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 mod n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 10) {
        return (seq_t)10 % (seq_t)n;
    } else {
        return (seq_t)10;
    }
}
