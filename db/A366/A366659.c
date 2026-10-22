/*
 * A366659  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(floor(n/k)+3,4).
 * https://oeis.org/A366659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binom(k+2,3) * (floor(n/k) mod 2)
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 572, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* add binomial only if floor(n/k) is odd */
        if ((n / k) & 1) {
            seq_t kseq = (seq_t)k;
            sum += (kseq * (kseq + 1) * (kseq + 2)) / 6;
        }
    }
    return sum;
}
