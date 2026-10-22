/*
 * A369630  Expansion of (1/x) * Series_Reversion( x * (1/(1+x^3) - x) ).
 * https://oeis.org/A369630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} binomial(2*n-3*k+1,k) *
 *            binomial(2*n-3*k,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_exact(idx_t n, idx_t k) {
    if (k > n) return 0;
    idx_t ksmall = (k < (n - k)) ? k : (n - k);
    seq_t res = 1;
    for (idx_t i = 1; i <= ksmall; ++i) {
        res *= (seq_t)(n - ksmall + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n1 = 2 * n - 3 * k;          /* 2*n-3*k  */
        seq_t b1 = binom_exact(n1 + 1, k);    /* binomial(2*n-3*k+1, k) */
        seq_t b2 = binom_exact(n1, n - 3 * k); /* binomial(2*n-3*k, n-3*k) */
        sum += b1 * b2;
    }
    return sum / ((seq_t)(n + 1));
}
