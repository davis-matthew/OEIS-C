/*
 * A339031  T(n, k) = k*P(n, k), where P(n, k) is the number of partitions of
 *          an n-set with k blocks, the largest of which has the size n - k +
 *          1. Triangle T(n, k) for 0 <= k <= n, read by rows.
 * https://oeis.org/A339031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle * T(r,c) read by rows,
 *            where * r = largest integer with r*(r+1)/2 <= n * c = n -
 *            r*(r+1)/2 (0 <= c <= r) * T(0,0) = 1 * For r > 0: * if c == 0 ->
 *            T = 0 * if c == r -> T = r * otherwise -> T = c * binomial(r,
 *            c-1)
 * Range:     u8 n <= 40, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0, hi = 1;
    /* exponential search to find an upper bound */
    while ((__int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    /* binary search in [lo, hi) */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t r = lo;
    idx_t prefix = (__int128)r * (r + 1) / 2;
    idx_t c = n - prefix;

    if (r == 0) return (seq_t)1;          /* T(0,0) */
    if (c == 0) return (seq_t)0;          /* T(r,0) for r>0 */
    if (c == r) return (seq_t)r;          /* T(r,r) */

    /* 1 <= c <= r-1 */
    seq_t bin = binomial(r, c - 1);
    seq_t result = bin * (seq_t)c;        /* multiplication in seq_t */
    return result;
}
