/*
 * A389376  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(n-1,n-3*k).
 * https://oeis.org/A389376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *            binomial(n-1,n-3*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 1; k <= maxk; ++k) {
        /* C(n + k - 1, k) */
        seq_t c1 = binom_small(n + k - 1, k);

        /* C(n - 1, 3*k - 1) */
        idx_t m = 3 * k - 1;            /* m ≥ 2 for k ≥ 1 */
        if (m > n - 1) continue;       /* term is zero, but loop bound already guarantees 3k ≤ n */
        seq_t c2 = binom_small(n - 1, m);

        sum += c1 * c2;
    }

    return sum;
}
