/*
 * A341704  a(n) = 20*binomial(n,6) + 2*binomial(n,3) + 1.
 * https://oeis.org/A341704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*binomial(n,6) + 2*binomial(n,3) + 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 75, u64 n <= 2956,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t n_choose_k(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t binom6 = n >= 6 ? n_choose_k(n, 6) : 0;
    seq_t binom3 = n >= 3 ? n_choose_k(n, 3) : 0;
    return ((seq_t)20) * binom6 + ((seq_t)2) * binom3 + (seq_t)1;
}
