/*
 * A244781  Number of length n 0..1 arrays with each partial sum starting from
 *          the beginning no more than one standard deviation from its mean.
 * https://oeis.org/A244781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n whose partial sums
 *            S_k satisfy |2*S_k - k| ≤ floor(sqrt(k)) for all 1 ≤ k ≤ n. This
 *            is computed by dynamic programming
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square root: largest r with r*r ≤ n. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > n / mid) {  /* avoid overflow */
            hi = mid - 1;
        } else {
            if (mid * mid <= n)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Maximum allowed |D_k| is floor(sqrt(n)). */
    idx_t maxR = isqrt_idx(n);
    size_t L = (size_t)(maxR * 2 + 1);   /* array length for states -maxR..+maxR */
    size_t shift = (size_t)maxR;         /* index offset for D = 0 */

    /* Allocate two arrays of size L for dynamic programming. */
    seq_t *cur  = (seq_t *)malloc(L * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(L * sizeof(seq_t));
    if (!cur || !next) {
        /* If allocation fails, return 0 as a fallback. */
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* Initialize all entries to zero. */
    for (size_t i = 0; i < L; ++i) {
        cur[i] = 0;
        next[i] = 0;
    }
    cur[shift] = (seq_t)1;  /* start from D_0 = 0 */

    /* Main DP loop: process each position k = 0 .. n-1. */
    for (idx_t k = 0; k < n; ++k) {
        idx_t bound_k  = isqrt_idx(k);
        idx_t bound_k1 = isqrt_idx(k + 1);

        /* Clear next array. */
        for (size_t i = 0; i < L; ++i)
            next[i] = 0;

        /* Iterate over all possible D_k values at step k. */
        for (long long d = -(long long)bound_k; d <= (long long)bound_k; ++d) {
            size_t idx = shift + d;
            seq_t cnt = cur[idx];
            if (cnt == 0)
                continue;  /* skip zero counts to save work */

            /* Step with X = 1: D_{k+1} = d + 1 */
            long long new_d = d + 1;
            if (new_d <= (long long)bound_k1 && new_d >= -(long long)bound_k1) {
                size_t nidx = shift + new_d;
                next[nidx] += cnt;
            }

            /* Step with X = 0: D_{k+1} = d - 1 */
            new_d = d - 1;
            if (new_d <= (long long)bound_k1 && new_d >= -(long long)bound_k1) {
                size_t nidx = shift + new_d;
                next[nidx] += cnt;
            }
        }

        /* Swap cur and next for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all valid final states. */
    seq_t result = 0;
    idx_t bound_n = isqrt_idx(n);
    for (long long d = -(long long)bound_n; d <= (long long)bound_n; ++d) {
        size_t idx = shift + d;
        result += cur[idx];
    }

    /* Free the allocated arrays. */
    free(cur);
    free(next);

    return result;
}
