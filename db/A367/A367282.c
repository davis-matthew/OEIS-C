/*
 * A367282  G.f. satisfies A(x) = 1 + x*A(x)^2 * (1 + x*A(x)^2)^2.
 * https://oeis.org/A367282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+1, k) * binomial(2*k, n-k) /
 *            (2*n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;          /* use symmetry to reduce k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t denom = (seq_t)((seq_t)n * 2 + 1);   /* 2*n + 1, cast to seq_t */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binom_small(2*n + 1, k);
        seq_t t2 = binom_small(2*k, n - k);
        sum += t1 * t2;
    }
    return sum / denom;   /* exact division */
}
