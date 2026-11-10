/*
 * A338276  a(n) is the number of odd terms in the n-th column of A334016.
 * https://oeis.org/A338276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of odd T(i,n), i>=1, where T = A334016 (T(1,1)=1,
 *            T(n,k)=Sum_{i=1..k-1} T(n+i,k-i) + Sum_{i=1..min(n,k)-1}
 *            T(n-i,k-i) + Sum_{i=1..n-1} T(n-i,k)); computed mod 2 with
 *            antidiagonal/diagonal/column prefix parities, rows capped at
 *            3K+8 (empirical bound)
 * Range:     u8 n <= 254, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 83 terms through n = 7588 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t cached = 0;
    static idx_t *cnt = NULL;

    if (n > cached) {
        idx_t K = 2 * cached;
        if (K < n) K = n;
        if (K < 64) K = 64;
        idx_t R = 3 * K + 8;
        idx_t M = R + K + 3;
        unsigned char *AD = (unsigned char *)calloc((size_t)(M + K + 3), 1);
        unsigned char *DG = (unsigned char *)calloc((size_t)(M + 2 * K + 6), 1);
        idx_t *nc = (idx_t *)calloc((size_t)(K + 1), sizeof(idx_t));
        for (idx_t k = 1; k <= K; ++k) {
            unsigned char col = 0;
            idx_t rows = R + K - k;
            idx_t c = 0;
            for (idx_t r = 1; r <= rows; ++r) {
                idx_t s = r + k;
                idx_t d = r + K - k;   /* r-k+K, nonnegative since K>=k */
                unsigned char t;
                if (k == 1 && r == 1) t = 1;
                else t = (unsigned char)(AD[s] ^ DG[d] ^ col);
                AD[s] ^= t;
                DG[d] ^= t;
                col ^= t;
                c += t;
            }
            nc[k] = c;
        }
        free(AD);
        free(DG);
        if (cnt) free(cnt);
        cnt = nc;
        cached = K;
    }
    return (seq_t)cnt[n];
}
