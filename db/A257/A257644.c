/*
 * A257644  First differences of A264100.
 * https://oeis.org/A257644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{m=1..n} (sigma(m)+tau(m)) = 1 + Sum_{d=1..n}
 *            (d+1)*floor(n/d)
 * Range:     u8 n <= 15, u16 n <= 278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9742 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 1;
    for (idx_t d = 1; d <= n; d++) {
        total += ((seq_t)d + 1) * (seq_t)(n / d);
    }
    return total;
}
