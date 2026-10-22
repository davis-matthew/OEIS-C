/*
 * A343124  Total number of partitions of k*n into 3 parts for k = 1..n.
 * https://oeis.org/A343124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} floor(((k*n)^2 + 6)/12)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 172, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)k * (seq_t)n;
        seq_t m2 = m * m;
        seq_t val = (m2 + (seq_t)6) / 12;
        sum += val;
    }
    return sum;
}
