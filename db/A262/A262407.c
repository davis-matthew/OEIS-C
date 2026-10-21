/*
 * A262407  a(n) = Sum_{k=0..n-1} C(n,k+1)*C(n,k)*C(n-1,k).
 * https://oeis.org/A262407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} C(n,k+1) * C(n,k) * C(n-1,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t res = 0;
    seq_t binom_nk   = 1;            /* C(n,0) */
    seq_t binom_nkp1 = (seq_t)n;     /* C(n,1) */
    seq_t binom_nm1k = 1;            /* C(n-1,0) */

    for (idx_t k = 0; k < n; ++k) {
        /* term = binom(n,k+1) * binom(n,k) * binom(n-1,k) */
        seq_t term = binom_nkp1;
        term = term * binom_nk;
        term = term * binom_nm1k;
        res += term;

        if (k == n - 1)
            break; /* last iteration done */

        /* Prepare values for the next k */
        seq_t next_nkp1 = binom_nkp1 * (seq_t)(n - (k + 1)) / (k + 2);   /* C(n,k+2) */
        seq_t next_nm1k = binom_nm1k * (seq_t)((n - 1) - k) / (k + 1);      /* C(n-1,k+1) */

        binom_nk   = binom_nkp1;          /* C(n,k+1) becomes the new C(n,k)   */
        binom_nkp1 = next_nkp1;           /* C(n,k+2) becomes the new C(n,k+1)*/
        binom_nm1k = next_nm1k;           /* C(n-1,k+1) becomes the new C(n-1,k) */
    }
    return res;
}
