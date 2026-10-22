/*
 * A361086  a(n) = a(n-1)*(a(n-1)^2 - 1) with a(0) = 2.
 * https://oeis.org/A361086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(0) = 2, a(n) = a(n-1)*(a(n-1)^2 - 1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)2;
    }
    seq_t prev = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq     = prev * prev;          /* a(n-1)^2          */
        seq_t factor = sq - (seq_t)1;        /* a(n-1)^2 - 1      */
        prev = prev * factor;               /* a(n-1)*(a(n-1)^2-1) */
    }
    return prev;
}
