/*
 * A378156  G.f. A(x) satisfies A(x) = ( 1 + x * A(x)^(1/2) * (1 + A(x)^(1/2))
 *          )^4.
 * https://oeis.org/A378156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..n} binomial(n,k) *
 *            binomial(2*n+2*k+4,n)/(n+k+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k, i;
    seq_t sum = 0;
    seq_t bin_n_k = 1;                 /* binomial(n, k)      */
    idx_t M = 2 * n + 4;               /* M_k = 2*n + 2*k + 4 */
    seq_t binom2 = 1;                  /* binomial(M, n)      */

    /* initial binom2 = binomial(M, n) */
    for (i = 1; i <= n; ++i) {
#ifdef __SIZEOF_INT128__
        unsigned __int128 t = (unsigned __int128)binom2 * (M - n + i);
        binom2 = (seq_t)(t / i);
#else
        binom2 = binom2 * (seq_t)(M - n + i) / (seq_t)i;
#endif
    }

    for (k = 0; k <= n; ++k) {
        seq_t term = bin_n_k * binom2;

#ifdef __SIZEOF_INT128__
        unsigned __int128 t = (unsigned __int128)term * 2;
        term = (seq_t)(t / (n + k + 2));
#else
        term = term * 2 / (seq_t)(n + k + 2);
#endif
        sum += term;

        if (k == n) break;

        /* next binomial(n, k+1)        */
#ifdef __SIZEOF_INT128__
        unsigned __int128 t1 = (unsigned __int128)bin_n_k * (n - k);
        bin_n_k = (seq_t)(t1 / (k + 1));
#else
        bin_n_k = bin_n_k * (seq_t)(n - k) / (seq_t)(k + 1);
#endif

        /* update M_k for next iteration  */
        M += 2;

        /* update binom2 = binomial(M, n) from previous value */
#ifdef __SIZEOF_INT128__
        unsigned __int128 num = (unsigned __int128)binom2 * (M - 1) * M;
        unsigned __int128 den = (unsigned __int128)(M - 1 - n) * (M - n);
        binom2 = (seq_t)(num / den);
#else
        binom2 = binom2 * (seq_t)(M - 1) * (seq_t)M /
                 ((seq_t)(M - 1 - n) * (seq_t)(M - n));
#endif
    }

    return sum;
}
