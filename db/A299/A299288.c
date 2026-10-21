/*
 * A299288  Partial sums of A299287.
 * https://oeis.org/A299288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (62*n^3 + 93*n^2 + 82*n + 24) / 24 for n even a(n) =
 *            (62*n^3 + 93*n^2 + 82*n + 27) / 24 for n odd Recurrence (for
 *            n>4): a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 458, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    if ((n & 1) == 0) {
        return (62 * m3 + 93 * m2 + 82 * m + 24) / 24;
    } else {
        return (62 * m3 + 93 * m2 + 82 * m + 27) / 24;
    }
}
