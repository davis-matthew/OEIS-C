/*
 * A321016  Triangle read by rows: number of partitions of n into distinct and
 *          consecutive parts with largest part k (n >= 1, 1 <= k <= n).
 * https://oeis.org/A321016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of m (1 ≤ m ≤ k) such that m*(2*k - m + 1)/2 = r,
 *            where r and k are obtained from the flattened index n by
 *            r*(r-1)/2 < n ≤ r*(r+1)/2 (row r, column k)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: find the smallest r with r*(r+1)/2 ≥ n.  Uses binary search and
   unsigned __int128 to avoid overflow. */
static idx_t triangular_row(idx_t n)
{
    idx_t lo = 1, hi = n;          /* hi is always ≥ desired r */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* compute mid*(mid+1)/2 safely */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Helper: given a flattened index n, obtain row r (sum) and column k
   (largest part).  Both r and k are ≥ 1 for n ≥ 1. */
static void row_col_from_index(idx_t n, idx_t *r, idx_t *k)
{
    if (n == 0) {                 /* out of range */
        *r = 0;
        *k = 0;
        return;
    }
    idx_t rr = triangular_row(n);
    unsigned __int128 before = (unsigned __int128)(rr - 1) * rr / 2;
    idx_t kk = n - (idx_t)before; /* 1‑based column */
    *r = rr;
    *k = kk;
}

/* Count how many consecutive-part partitions of sum r have largest part k.
   This is exactly the number of lengths m (1 ≤ m ≤ k) satisfying
   m*(2*k - m + 1)/2 = r. */
static seq_t count_partitions(idx_t r, idx_t k)
{
    seq_t cnt = 0;
    for (idx_t m = 1; m <= k; ++m) {
        unsigned __int128 lhs = (unsigned __int128)m * (2 * k - m + 1);
        if (lhs == (unsigned __int128)2 * r)
            ++cnt;
    }
    return cnt;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 for safety */
        return 0;

    idx_t r, k;
    row_col_from_index(n, &r, &k);
    return count_partitions(r, k);
}
