/*
 * A376811  Expansion of 1/sqrt(1 - 4*x/(1 - x^3)^2).
 * https://oeis.org/A376811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-5*k-1,k) *
 *            binomial(2*n-6*k,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_exact(idx_t n, idx_t k)
{
    if (k == 0 || n == k) return (seq_t)1;
    if (n < k) return (seq_t)0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1;
        if (k == 0) {
            term1 = 1;
        } else {
            if (2 * n < 5 * k + 1) {
                term1 = 0;
            } else {
                idx_t first_n = 2 * n - 5 * k - 1;
                term1 = binom_exact(first_n, k);
            }
        }
        seq_t term2 = binom_exact(2 * n - 6 * k, n - 3 * k);
        sum += term1 * term2;
    }
    return sum;
}
