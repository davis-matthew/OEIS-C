/*
 * A266945  Expansion of Product_{k>=1} 1 / (1 - 2*x^k)^3.
 * https://oeis.org/A266945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of xⁿ in ∏_{k≥1} 1/(1-2xᵏ)³
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 49, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses dynamic programming to compute the
   weighted partition number with multiplicities weighted by
   binom(m+2,2)·2^m for each part size k.  All arithmetic is done
   in seq_t; memory is allocated with new/delete in C++ or
   malloc/free in C. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t size = n + 1;

#if defined(__cplusplus)
    seq_t *dp    = new seq_t[size];
    seq_t *newdp = new seq_t[size];
#else
    seq_t *dp    = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc(size * sizeof(seq_t));
#endif

    /* initialise dp */
    for (idx_t i = 0; i < size; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxm = n / k;    /* largest multiplicity of part k */

        /* w[m] = binom(m+2,2) * 2^m */
#if defined(__cplusplus)
        seq_t *w = new seq_t[maxm + 1];
#else
        seq_t *w = (seq_t *)malloc((maxm + 1) * sizeof(seq_t));
#endif
        w[0] = 1;
        seq_t pow2 = 1;
        for (idx_t m = 1; m <= maxm; ++m) {
            pow2 = pow2 * 2;
            seq_t binom = ((seq_t)(m + 2)) * ((seq_t)(m + 1)) / (seq_t)2;
            w[m] = pow2 * binom;
        }

        /* zero newdp */
        for (idx_t i = 0; i < size; ++i) newdp[i] = 0;

        /* convolution of dp by the series for part k */
        for (idx_t i = 0; i < size; ++i) {
            seq_t dpi = dp[i];
            if (dpi == 0) continue;
            idx_t mmax = (n - i) / k;
            for (idx_t m = 0; m <= mmax; ++m) {
                idx_t j = i + m * k;
                newdp[j] += dpi * w[m];
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;

#if defined(__cplusplus)
        delete [] w;
#else
        free (w);
#endif
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete [] dp;
    delete [] newdp;
#else
    free (dp);
    free (newdp);
#endif

    return result;
}
