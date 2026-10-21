/*
 * A282852  37-gonal numbers: a(n) = n*(35*n-33)/2.
 * https://oeis.org/A282852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(35*n-33)/2
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t nn = (seq_t) n;
    seq_t result = nn * ((seq_t)nn * 35 - 33) / 2;
    return result;
}
