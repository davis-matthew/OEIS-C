/*
 * A247610  a(n) = Sum_{k=0..5} binomial(10,k)*binomial(n,k).
 * https://oeis.org/A247610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..5} binomial(10,k)*binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    const int coeff[6] = {1, 10, 45, 120, 210, 252};
    seq_t sum = 0;
    for (idx_t k = 0; k <= 5; ++k) {
        sum += (seq_t)coeff[k] * binom_small(n, k);
    }
    return sum;
}
