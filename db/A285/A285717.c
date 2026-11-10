/*
 * A285717  a(n) = A007814(n) + A159918(n) = A007814(n) + A000120(n^2).
 * https://oeis.org/A285717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007814(n) + A000120(n^2), i.e. 2-adic valuation of n
 *            plus popcount of n^2
 * Range:     u8 n <= 15, u16 n <= 387, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 136 terms through n = 7018 (2026-11-08)
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

static seq_t A000120_a(seq_t n) {
    seq_t c = 0;
    while (n) {
        c += (seq_t)(n & 1);
        n >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t sq = (seq_t)n * (seq_t)n;
    return A007814_a(n) + A000120_a(sq);
}
