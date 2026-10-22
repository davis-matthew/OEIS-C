/*
 * A381677  a(n) = floor(a(n-1)*3/2) with a(1) = 5.
 * https://oeis.org/A381677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(a(n-1)*3/2), a(1) = 5
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)5;
    seq_t val = (seq_t)5;
    for (idx_t i = 2; i <= n; ++i)
        val = (val * (seq_t)3) / 2;
    return val;
}
