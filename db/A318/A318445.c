/*
 * A318445  a(n) = Sum_{d|n, d<n} A005187(d).
 * https://oeis.org/A318445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A005187(d), where A005187(m) = Sum_{k>=0}
 *            floor(m/2^k) = 2m - popcount(m)
 * Range:     u8 n <= 83, u16 n <= 12599, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 101 terms through n = 9920 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A005187_a(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (d < n) total += A005187_a(d);
            if (e != d && e < n) total += A005187_a(e);
        }
    }
    return total;
}
