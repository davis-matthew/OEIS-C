/*
 * A255827  a(n) = n for n < 7; a(7n) = a(n); if every 7th term (a(7), a(14),
 *          a(21),...) is deleted, this gives back the original sequence.
 * https://oeis.org/A255827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n < 7; a(n) = a(n/7) if n % 7 == 0; otherwise a(n)
 *            = a(n - floor(n/7))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 7) return (seq_t)n;
    while (n >= 7) {
        if ((n % 7) == 0) {
            n /= 7;
        } else {
            n -= (n / 7);
        }
    }
    return (seq_t)n;
}
