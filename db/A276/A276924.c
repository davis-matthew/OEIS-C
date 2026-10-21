/*
 * A276924  Number of ordered set partitions of [n] with at most four elements
 *          per block.
 * https://oeis.org/A276924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1}^{min(4,n)} binom(n,k) * a(n-k) for n ≥
 *            1 (Number of ordered set partitions of [n] with blocks of size
 *            at most 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t small_binom(idx_t n, int k)
{
    seq_t res = 1;
    for (int t = 1; t <= k; ++t)
        res = res * (n - t + 1) / t;
    return res;
}

seq_t a(idx_t n)
{
    /* buffer holds the last four computed values:
       buf[j] == a(j) for j ≡ current_index (mod 4) */
    seq_t buf[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0 };

    if (n == 0)
        return buf[0];

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t val = 0;
        for (int k = 1; k <= 4; ++k)
        {
            if (i < (idx_t)k)
                continue;
            val += small_binom(i, k) * buf[(i - k) % 4];
        }
        buf[i % 4] = val;
    }
    return buf[n % 4];
}
