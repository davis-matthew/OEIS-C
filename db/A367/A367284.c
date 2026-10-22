/*
 * A367284  G.f. satisfies A(x) = 1 + x*A(x) * (1 + x*A(x)^3)^3.
 * https://oeis.org/A367284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binom(3n-2k+1, k) * binom(3k, n-k) / (3n-2k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        /* Use 128‑bit intermediate to avoid overflow of the product */
        r = ( (unsigned __int128) r * num ) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t total = 0;
    idx_t k_start = (n + 3) / 4;            /* ceil(n/4) */
    for (idx_t k = k_start; k <= n; ++k) {
        idx_t m1 = 3 * n - 2 * k + 1;       /* 3n-2k+1  */
        idx_t m2 = 3 * k;                   /* 3k       */
        idx_t k2 = n - k;                   /* n-k      */

        seq_t B = binom(m1, k);
        seq_t C = binom(m2, k2);
        seq_t denom = (seq_t)m1;

        seq_t term = ( (unsigned __int128) B * C ) / denom;
        total += term;
    }
    return total;
}
