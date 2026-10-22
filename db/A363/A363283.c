/*
 * A363283  Squares (A000290) and (1+squares) (A002522), in increasing order.
 * https://oeis.org/A363283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n^2+2n+5)-(2n-3)*(-1)^n)/8, * which simplifies to: * n
 *            even : a(n) = (n^2 + 4) / 4 * n odd : a(n) = ((n+1)^2) / 4
 * Range:     u8 n <= 14, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n & 1U) {                // odd n
        seq_t m = (seq_t)n + 1U;
        seq_t sq = m * m;       // (n+1)^2
        return sq / 4U;
    } else {                     // even n
        seq_t sn = (seq_t)n;
        seq_t sq = sn * sn;     // n^2
        return (sq + 4U) / 4U;
    }
}
