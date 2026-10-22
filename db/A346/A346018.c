/*
 * A346018  Triangle read by rows: T(n,k) is the number of integers m such
 *          that 1 <= m <= n and gcd(m,n) <= k, 1 <= k <= n.
 * https://oeis.org/A346018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th element of the triangle read by rows,
 * where T(n,k) = #{m : 1 ≤ m ≤ n, gcd(m,n) ≤ k}.  The sequence
 * a(n) is obtained by flattening the triangle row by row.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
    idx_t low = 1, high = 1;
    /* Grow high until it covers n. */
    while ((high * (high + 1) / 2) < n)
        high *= 2;

    /* Binary search for the smallest r with r*(r+1)/2 ≥ n. */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if ((mid * (mid + 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position k within the row (1‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;  /* 1 ≤ k ≤ r */

    /* Count m where gcd(m,r) ≤ k. */
    seq_t count = 0;
    for (idx_t m = 1; m <= r; ++m)
    {
        if (gcd((seq_t)m, (seq_t)r) <= (seq_t)k)
            ++count;
    }
    return count;
}
