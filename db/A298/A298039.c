/*
 * A298039  Partial sums of A298038.
 * https://oeis.org/A298039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 18*(⌊n/2⌋)^2 + 12*⌊n/2⌋ if n is even, a(n) =
 *            18*(⌊n/2⌋)^2 + 24*⌊n/2⌋ + 7 if n is odd
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n >> 1;          /* floor(n/2) */
    if ((n & 1ULL) == 0) {     /* even n */
        seq_t mm = (seq_t)m;
        return (seq_t)1 + ((seq_t)18 * mm * mm) + ((seq_t)12 * mm);
    } else {                   /* odd n */
        seq_t mm = (seq_t)m;
        return ((seq_t)18 * mm * mm) + ((seq_t)24 * mm) + (seq_t)7;
    }
}
