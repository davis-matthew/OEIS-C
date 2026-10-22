/*
 * A389060  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(3*k,n-2*k).
 * https://oeis.org/A389060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(3*k ,
 *            n-2*k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes binomial coefficient C(n,k) using multiplicative formula */
static seq_t binom_iter(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i;  the division is exact */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* special case n == 0 handled naturally in the loop */
    idx_t limit = n >> 1;             /* floor(n/2) */
    seq_t ans   = 0;

    seq_t bin_nk = 1;                 /* C(n,0) */
    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t m = 3 * k;
        idx_t r = n - 2 * k;
        seq_t bin_mr = 0;
        if (r <= m) bin_mr = binom_iter(m, r);

        /* add term for current k */
        ans += bin_nk * bin_mr;

        /* update bin_nk for next k (if any) */
        if (k < limit)
            bin_nk = bin_nk * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return ans;
}
