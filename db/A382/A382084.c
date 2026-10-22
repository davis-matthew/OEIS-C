/*
 * A382084  a(n) = 90*binomial(n,6) + 18*binomial(n,4) + 3*binomial(n,2) + 1.
 * https://oeis.org/A382084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*binomial(n,6) + 18*binomial(n,4) + 3*binomial(n,2) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_local(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * ((seq_t)n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t b2 = binom_local(n, 2);
    seq_t b4 = binom_local(n, 4);
    seq_t b6 = binom_local(n, 6);
    seq_t term = (seq_t)90 * b6 + (seq_t)18 * b4 + (seq_t)3 * b2 + (seq_t)1;
    return term;
}
