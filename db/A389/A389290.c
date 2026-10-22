/*
 * A389290  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(k,n-3*k).
 * https://oeis.org/A389290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n+k-1,k) * binomial(k,n-3k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* compute res = res * (n - k + i) / i; */
        seq_t prod = res * (seq_t)(n - k + i);
        res = prod / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t term = n - 3 * k;          /* ≥ 0 by loop bound */
        seq_t b1 = binom_mul(n + k - 1, k);
        seq_t b2 = binom_mul(k, term);
        sum += b1 * b2;
    }
    return sum;
}
