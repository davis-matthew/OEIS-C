/*
 * A328361  Triangle read by rows: T(n,k) is the total number of k's in all
 *          partitions of n into consecutive parts, (1 <= k <= n).
 * https://oeis.org/A328361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328361: flatten the triangle of consecutive-part partitions.
   The index n is first used to locate the row r and column k of the
   triangle (flattened in row-major order).  The value a(n) is the number
   of partitions of r into consecutive parts that contain the part k.
   The count is obtained by enumerating all divisors m of 2r and testing
   whether k lies in the interval [a, a+m-1], where a = (2r/m - m + 1)/2. */

#include <stdint.h>
#include <limits.h>

/* Helper: find the row r such that r(r+1)/2 >= idx,
   where idx is a 1-based flattened index. */
static inline idx_t
row_from_index(idx_t idx)
{
    if (idx == 0)
        return 0;
    idx_t low = 1, high = 1;
    /* Increase high until it is large enough */
    while (((unsigned __int128)high * (high + 1)) / 2 < idx) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    /* Binary search for minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main function that implements a(n) for OEIS A328361. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of offset range, harmless */

    /* Determine the row r and column k within that row. */
    idx_t r = row_from_index(n);
    idx_t prev_rows = ((idx_t)((unsigned __int128)(r - 1) * r / 2));
    idx_t k = n - prev_rows;           /* 1‑based column index in row r */

    /* Count the partitions of r into consecutive parts that contain k. */
    idx_t count = 0;
    idx_t t = 2 * r;                    /* t = 2r */

    for (idx_t d = 1; d * d <= t; ++d) {
        if (t % d != 0)
            continue;
        /* Process divisor m = d */
        {
            idx_t m = d;
            int64_t num = (int64_t)(t / m) - (int64_t)m + 1;
            if ((num & 1LL) == 0) {        /* check evenness */
                int64_t a_start = num / 2; /* a must be >=1 */
                if (a_start >= 1) {
                    int64_t end = a_start + (int64_t)m - 1;
                    if ((int64_t)k >= a_start && (int64_t)k <= end)
                        ++count;
                }
            }
        }
        /* Process the paired divisor if distinct */
        if (d * d == t)
            continue;                     /* avoid double counting */
        {
            idx_t m = t / d;
            int64_t num = (int64_t)(t / m) - (int64_t)m + 1;
            if ((num & 1LL) == 0) {
                int64_t a_start = num / 2;
                if (a_start >= 1) {
                    int64_t end = a_start + (int64_t)m - 1;
                    if ((int64_t)k >= a_start && (int64_t)k <= end)
                        ++count;
                }
            }
        }
    }
    return (seq_t)count;
}
