/*
 * A378569  a(n) = 3*n*(n+1) + 7.
 * https://oeis.org/A378569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n+1) + 7
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);
    prod = prod * 3;
    return prod + (seq_t)7;
}
