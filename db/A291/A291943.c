/*
 * A291943  a(0)=0; for n>0, a(n) = (2n)-th digit after the decimal point in
 *          the decimal expansion of 1/(2n+1).
 * https://oeis.org/A291943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 3817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (2n)-th digit after the decimal point in decimal expansion of 1/(2n+1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t denom = (seq_t)n * (seq_t)2 + 1;
    seq_t rem   = 1;
    seq_t digit = 0;
    idx_t limit = (idx_t)((seq_t)n * (seq_t)2);
    for (idx_t i = 0; i < limit; ++i) {
        rem = rem * 10;
        digit = rem / denom;
        rem   = rem % denom;
    }
    return digit;
}
