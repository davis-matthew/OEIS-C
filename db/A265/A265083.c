/*
 * A265083  Column 3 of array in A265080.
 * https://oeis.org/A265083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..n} Σ_{j=0..n-i} max(i, j, n-i-j) * binom(n, i) *
 *            binom(n-i, j) (binom denotes the binomial coefficient)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t bin1 = 1;                     /* binom(n, 0) */
    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t rem = n - i;               /* remaining part for j and k */
        seq_t bin2 = 1;                 /* binom(n-i, 0) */
        for (idx_t j = 0; j <= rem; ++j)
        {
            idx_t k = rem - j;           /* third part, i + j + k = n */
            idx_t maxv = i;
            if (j > maxv) maxv = j;
            if (k > maxv) maxv = k;

            total += bin1 * bin2 * (seq_t)maxv;

            if (j < rem)
                bin2 = bin2 * (seq_t)(k) / (seq_t)(j + 1);  /* next binom(n-i, j+1) */
        }
        if (i < n)
            bin1 = bin1 * (seq_t)(n - i) / (seq_t)(i + 1);   /* next binom(n, i+1) */
    }
    return total;
}
