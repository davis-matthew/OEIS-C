/*
 * A350143  a(n) = Sum_{k=1..n} floor(n/(2*k-1))^2.
 * https://oeis.org/A350143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/(2*k-1))^2
 * Range:     u8 n <= 14, u16 n <= 231, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t n_s = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t d = 2*k - 1;
        seq_t q = n_s / (seq_t)d;
        sum += q * q;
    }
    return sum;
}
