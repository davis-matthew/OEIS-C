/*
 * A276921  Number A(n,k) of ordered set partitions of [n] with at most k
 *          elements per block; square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A276921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the table A(n,k) of * ordered
 *            set partitions of [n] with each block size ≤ k. * The table
 *            satisfies A(0,k)=1 and for n≥1, * A(n,k)=∑_{s=1}^{min(k,n)}
 *            C(n,s)·A(n‑s,k). * The function returns the element a(n)
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 116, u64 n <= 249,
 *            u128 n <= 625, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"

static seq_t *dp = NULL;
static size_t dp_stride = 0;

/* Find the largest d such that d*(d+1)/2 ≤ n */
static idx_t find_d(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
        {
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Recompute the DP table up to size max_n (inclusive) */
static void ensure_dp(size_t max_n)
{
    size_t new_stride = max_n + 1;
    if (dp_stride != 0 && dp_stride >= new_stride) return;

    seq_t *new_dp = (seq_t *)calloc(new_stride * new_stride, sizeof(seq_t));
    if (!new_dp) return; /* allocation failure – caller sees zero values */

    /* base row n==0 */
    for (size_t k = 0; k < new_stride; ++k)
        new_dp[0 * new_stride + k] = 1;

    /* compute remaining cells */
    for (size_t n = 1; n < new_stride; ++n)
    {
        for (size_t k = 0; k < new_stride; ++k)
        {
            seq_t sum = 0;
            size_t maxS = (k < n) ? k : n;
            for (size_t s = 1; s <= maxS; ++s)
            {
                seq_t choose = binom((idx_t)n, (idx_t)s);
                seq_t prod  = choose * new_dp[(n - s) * new_stride + k];
                sum += prod;
            }
            new_dp[n * new_stride + k] = sum;
        }
    }

    free(dp);
    dp = new_dp;
    dp_stride = new_stride;
}

seq_t a(idx_t n)
{
    /* map n to coordinates (i,j) of the antidiagonal table */
    idx_t d = find_d(n);
    unsigned __int128 t = (unsigned __int128)d * (d + 1) / 2;
    idx_t sum_d = (idx_t)t;
    idx_t r = n - sum_d;
    idx_t i = r;
    idx_t j = d - r;

    /* ensure DP table covers needed indices */
    size_t need = (i > j) ? i : j;
    ensure_dp(need);

    return dp[i * dp_stride + j];
}
