/*
 * A323496  a(n) = Product_{k=1..n} (binomial(k-1,3) + binomial(n-k,3)).
 * https://oeis.org/A323496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (binomial(k-1,3) + binomial(n-k,3))
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) = 1 */

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t j1 = k - 1;
        idx_t j2 = n - k;

        seq_t t1 = 0;
        if (j1 >= 3)
            t1 = (seq_t)j1 * (j1 - 1) * (j1 - 2) / 6;

        seq_t t2 = 0;
        if (j2 >= 3)
            t2 = (seq_t)j2 * (j2 - 1) * (j2 - 2) / 6;

        seq_t sum = t1 + t2;
        if (sum == 0)        /* early exit for zero factors */
            return 0;

        prod *= sum;
    }
    return prod;
}
