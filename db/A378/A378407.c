/*
 * A378407  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) *
 *          binomial(n+2*k,n-3*k).
 * https://oeis.org/A378407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(n+2*k,
 *            n-3*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
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
    seq_t bn = 1;                     /* C(n,0) */
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t Bk = binom(n + 2 * k, n - 3 * k);
        sum += bn * Bk;
        if (k == maxk) break;
        /* update bn to C(n, k+1) */
        bn = bn * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return sum;
}
