/*
 * A286429  Highest elevation of an island above sea level in a number square.
 * https://oeis.org/A286429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 3; a(n) = n^2 + 2*n - 8 for n > 3
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)0;
    }
    seq_t tn = (seq_t)n;
    return tn * tn + tn * ((seq_t)2) - ((seq_t)8);
}
