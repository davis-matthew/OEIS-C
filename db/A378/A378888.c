/*
 * A378888  a(n) = gcd(n, A007814(n)).
 * https://oeis.org/A378888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, A007814(n)), where A007814(n) = 2-adic valuation
 *            of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 7915 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A007814_a(idx_t n) {
    seq_t count = 0;
    while ((n & 1) == 0) {
        count++;
        n >>= 1;
    }
    return count;
}

static seq_t A378888_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t v = A007814_a(n);
    if (v == 0) return (seq_t)n;
    return A378888_gcd((seq_t)n, v);
}
