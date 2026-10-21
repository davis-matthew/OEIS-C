/*
 * A269501  Subsequence immediately following the instances of n in the
 *          sequence is n, n-1, ..., 1, n+1, n+2, ....
 * https://oeis.org/A269501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (r^2 - n)/2 + 1 if n and r have the same parity,
 *            otherwise a(n) = r, where r = ceil(sqrt(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;   /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    idx_t f = floor_sqrt_u64(n);
    idx_t r = (f * f == n) ? f : f + 1;   /* ceil(sqrt(n)) */
    if (((n ^ r) & 1) == 0) {   /* n and r have the same parity */
        unsigned __int128 sq = (unsigned __int128)r * r;
        seq_t diff = (seq_t)(sq - n);   /* diff <= 2r and fits in seq_t */
        return (diff / 2) + 1;
    } else {
        return (seq_t)r;
    }
}
