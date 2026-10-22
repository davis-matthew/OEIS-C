/*
 * A336246  Array read by upwards antidiagonals: T(n,k) is the number of ways
 *          to place n persons on different seats such that each person number
 *          p, 1 <= p <= n, differs from the seat number s(p), 1 <= s(p) <=
 *          n+k, k >= 0.
 * https://oeis.org/A336246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m - off, off) where m = minimal integer with m*(m+1)/2
 *            >= n, off = n - (m-1)*m/2 - 1. T(0,k)=1; T(1,k)=k; T(n,k) =
 *            (n+k-1)*T(n-1,k) + (n-1)*T(n-2,k)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find m such that m*(m+1)/2 >= n */
    idx_t low  = 0;
    idx_t high = 1;
    while(((unsigned __int128)high * (high + 1)) / 2 < n)
        high <<= 1;
    while(low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if(((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t m = high;

    /* offset within the block */
    unsigned __int128 prev_tri = ((unsigned __int128)(m - 1) * m) / 2;
    idx_t off = n - (idx_t)prev_tri - 1;

    idx_t np = m - off;  /* the n argument for T */
    idx_t kp = off;      /* the k argument for T */

    /* dynamic programming for T(np, kp) */
    seq_t *row_nm2 = (seq_t*)malloc((kp + 1) * sizeof(seq_t));
    seq_t *row_nm1 = (seq_t*)malloc((kp + 1) * sizeof(seq_t));
    seq_t *row_n   = (seq_t*)malloc((kp + 1) * sizeof(seq_t));

    /* T(0,k) = 1 */
    for(idx_t k = 0; k <= kp; ++k)
        row_nm2[k] = (seq_t)1;

    /* T(1,k) = k */
    for(idx_t k = 0; k <= kp; ++k)
        row_nm1[k] = (seq_t)k;

    if(np == 0) {
        seq_t res = row_nm2[kp];
        free(row_nm2); free(row_nm1); free(row_n);
        return res;
    }
    if(np == 1) {
        seq_t res = row_nm1[kp];
        free(row_nm2); free(row_nm1); free(row_n);
        return res;
    }

    /* compute rows for n = 2 .. np */
    for(idx_t cur_n = 2; cur_n <= np; ++cur_n) {
        for(idx_t k = 0; k <= kp; ++k) {
            seq_t term1 = ((seq_t)(cur_n + k - 1)) * row_nm1[k];
            seq_t term2 = ((seq_t)(cur_n - 1)) * row_nm2[k];
            row_n[k] = term1 + term2;
        }
        /* rotate rows */
        seq_t *tmp = row_nm2;
        row_nm2 = row_nm1;
        row_nm1 = row_n;
        row_n   = tmp;
    }

    seq_t res = row_nm1[kp];
    free(row_nm2); free(row_nm1); free(row_n);
    return res;
}
