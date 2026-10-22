/*
 * A352047  Sum of the divisor complements of the odd proper divisors of n.
 * https://oeis.org/A352047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sum_{d|n, d<n, d odd} 1/d
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 != n && (d1 & 1)) {
                total += n_seq / (seq_t)d1;
            }
            if (d2 != d1 && d2 != n && (d2 & 1)) {
                total += n_seq / (seq_t)d2;
            }
        }
    }

    return total;
}
