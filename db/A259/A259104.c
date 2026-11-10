/*
 * A259104  A000522(n+2)-A000522(n).
 * https://oeis.org/A259104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000522(n+2) - A000522(n), where A000522(n) =
 *            n*A000522(n-1) + 1, A000522(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A259104_e(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result = (seq_t)i * result + 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t x = 1;
    seq_t prev = 1, cur = 1;
    (void)x;
    (void)prev;
    for (idx_t i = 1; i <= n + 2; i++) {
        seq_t next = (seq_t)i * cur + 1;
        if (i == n) prev = next;
        cur = next;
    }
    if (n == 0) prev = 1;
    return cur - prev;
}
