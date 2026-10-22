/*
 * A329146  Triangle read by rows: T(n,k) is the number of subsets of
 *          {1,...,n} such that the difference between any two elements is k
 *          or greater, 1 <= k <= n.
 * https://oeis.org/A329146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((√(8n+1)-1)/2), k = n - r(r-1)/2;
 *            T(n,k) satisfies T(n,k) = n+1 if n <= k T(n,k) = T(n-1,k) +
 *            T(n-k,k) otherwise
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 2016,
 *            u128 n <= 8128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  // for size_t
#include <stdlib.h>  // for malloc, free

/* Find the row number r such that r(r+1)/2 >= n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;   /* high never exceeds n */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute T(r,k) by dynamic programming using the recurrence. */
static inline seq_t compute_T(idx_t r, idx_t k)
{
    size_t R = (size_t)r;
    seq_t *dp = (seq_t *)malloc((R + 1) * sizeof(seq_t));
    /* base case for t <= k : T(t,k) = t+1 */
    size_t max_base = (k > R) ? R : (size_t)k;
    for (size_t t = 0; t <= max_base; ++t)
        dp[t] = (seq_t)(t + 1);
    /* recurrence for t > k */
    for (size_t t = max_base + 1; t <= R; ++t)
        dp[t] = dp[t - 1] + dp[t - k];
    seq_t result = dp[R];
    free(dp);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined by the OEIS offset; return 0 as a safe fallback */
        return 0;

    idx_t r = find_row(n);

    /* compute first index of row r: r*(r-1)/2 + 1 */
    unsigned __int128 start128 = (unsigned __int128)r * (r - 1) / 2 + 1;
    idx_t start = (idx_t)start128;

    idx_t k = n - start + 1;   /* column within the row */

    return compute_T(r, k);
}
