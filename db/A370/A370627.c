/*
 * A370627  a(n) = 2^(n - 1)*((-1)^(n + 1) + 7*2^n)/3 = 2^(n - 1)*A062092(n).
 * https://oeis.org/A370627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n - 1) + 8*a(n - 2) for n > 1, a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 5;
    seq_t prevprev = 1;
    seq_t prev = 5;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev << 1) + (prevprev << 3); /* 2*prev + 8*prevprev */
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
