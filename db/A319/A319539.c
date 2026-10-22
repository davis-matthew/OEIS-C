/*
 * A319539  Array read by antidiagonals: T(n,k) is the number of binary rooted
 *          trees with n leaves of k colors and all non-leaf nodes having
 *          out-degree 2.
 * https://oeis.org/A319539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 60, u32 n <= 130, u64 n <= 317,
 *            u128 n <= 847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(i,k) where T is defined as:
 *   T(1,k) = k
 *   T(n,k) = ( [n mod 2 == 0]*T(n/2,k) + Σ_{j=1}^{n-1} T(j,k)*T(n-j,k) ) / 2
 * Sequence a(n) lists A319539 by reading the pairs (n,k) in antidiagonal
 * order: for each sum s = n+k, list the pairs with n from 1 to s-1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Find smallest s with s*(s-1)/2 >= n (n >= 1) */
static idx_t find_t(idx_t n)
{
    idx_t lo = 2, hi = 4;
    while (( (__int128)hi * (hi - 1) ) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (( (__int128)mid * (mid - 1) ) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Global cache for T(n,k) */
static seq_t **T = NULL;
static idx_t cur_n_max = 0;   /* largest n stored */
static idx_t cur_k_max = 0;   /* largest k stored */

/* Build or enlarge the DP table up to N (leaves) and K (colors) */
static void build_table(idx_t N, idx_t K)
{
    if (N <= cur_n_max && K <= cur_k_max)
        return;  /* table already large enough */

    /* free old table */
    for (idx_t i = 1; i <= cur_n_max; ++i)
        free(T[i]);
    free(T);

    /* allocate new table */
    T = (seq_t **)calloc((size_t)(N + 1), sizeof(seq_t *));
    for (idx_t i = 1; i <= N; ++i)
        T[i] = (seq_t *)calloc((size_t)(K + 1), sizeof(seq_t));

    /* base case */
    for (idx_t k = 1; k <= K; ++k)
        T[1][k] = (seq_t)k;

    /* DP computation */
    for (idx_t n = 2; n <= N; ++n) {
        for (idx_t k = 1; k <= K; ++k) {
            __int128 sum = 0;
            for (idx_t j = 1; j < n; ++j) {
                sum += (__int128)T[j][k] * (__int128)T[n - j][k];
            }
            if ((n & 1) == 0)
                sum += (__int128)T[n / 2][k];
            T[n][k] = (seq_t)(sum / 2);
        }
    }

    cur_n_max = N;
    cur_k_max = K;
}

/* Public function to get a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out of offset */

    /* find diagonal sum s */
    idx_t s = find_t(n);

    /* number of pairs before this diagonal */
    idx_t prev = ( (s - 2) * (s - 1) ) / 2;

    /* offset within diagonal (0-based) */
    idx_t offset = n - prev - 1;

    /* convert to i (leaves) and k (colors) */
    idx_t i = offset + 1;
    idx_t k = s - i;

    /* ensure DP table covers i and k */
    build_table(i, k);

    return T[i][k];
}
