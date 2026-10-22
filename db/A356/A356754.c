/*
 * A356754  Triangle read by rows: T(n,k) = ((n-1)*(n+2))/2 + 2*k.
 * https://oeis.org/A356754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r is the row number of n in the triangle and
 *            k = n - r(r-1)/2. T(r,k) = ((r-1)(r+2))/2 + 2k. The row r is the
 *            smallest integer with r(r+1)/2 >= n. This is equivalent to
 *            reading the triangle row‑by‑row starting from a(1)=2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* increase hi until it is large enough */
    while (hi * (hi + 1) / 2 < n) {
        if (hi > (UINT64_MAX >> 1)) break; /* avoid overflow */
        hi <<= 1;
    }
    /* binary search for the smallest r with r(r+1)/2 >= n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) break; /* safety */
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for index 0, not used by OEIS */

    idx_t r = find_row(n);
    /* column index in row r (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* compute ((r-1)(r+2))/2 + 2*k using seq_t */
    seq_t part1 = ((seq_t)(r - 1) * (seq_t)(r + 2)) / 2;
    seq_t part2 = 2 * (seq_t)k;

    return part1 + part2;
}
