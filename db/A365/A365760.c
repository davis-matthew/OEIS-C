/*
 * A365760  G.f. A(x) satisfies A(x) = 1 + x*A(x)*(1 + x^4*A(x)^5).
 * https://oeis.org/A365760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{k=0..⌊n/5⌋} binomial(n-4*k, k) * binomial(n+k+1,
 *            n-4*k) / (n+k+1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* local binomial coefficient (works for all seq_t types) */
static inline seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;  /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide – division is exact */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 5; ++k) {
        seq_t c1 = binom_local(n - 4 * k, k);
        seq_t c2 = binom_local(n + k + 1, n - 4 * k);
        seq_t num = c1 * c2;
        seq_t den = (seq_t)(n + k + 1);
        seq_t term = num / den;
        sum += term;
    }
    return sum;
}
