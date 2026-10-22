/*
 * A366971  a(n) = Sum_{k=3..n} binomial(k,3) * floor(n/k).
 * https://oeis.org/A366971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n} binomial(k,3) * floor(n/k)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A366971. */
seq_t a(idx_t n) {
    if (n < 3) {
        return (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)3; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        /* binomial(k,3) = k*(k-1)*(k-2)/6 */
        seq_t bin = kk * (kk - 1) * (kk - 2) / 6;
        /* floor(n/k) computed on idx_t, then cast to seq_t */
        seq_t term = bin * (seq_t)(n / k);
        sum += term;
    }
    return sum;
}
