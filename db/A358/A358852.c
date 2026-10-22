/*
 * A358852  a(n) = n!*Sum_{m=0..floor(n/3)} 1/binomial(n-m,2*m).
 * https://oeis.org/A358852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{m=0..floor(n/3)} 1/binomial(n-m,2*m)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t factorial(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= n; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)            return 0;
    if (k > n - k)       k = n - k;   /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t nFact = factorial(n);
    idx_t mmax = n / 3;
    seq_t sum = 0;
    for (idx_t m = 0; m <= mmax; ++m) {
        seq_t denom = binomial(n - m, 2 * m);
        if (denom == 0) continue;          /* should not happen */
        seq_t term = nFact / denom;
        sum += term;
    }
    return sum;
}
