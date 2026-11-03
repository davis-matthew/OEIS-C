/*
 * A340804  Triangle read by rows: T(n, k) = 1 + k*(n - 1) + (2*k - n - 1)*(k
 *          mod 2) with 0 < k <= n.
 * https://oeis.org/A340804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n)=T(r,k) where r is the smallest integer with r(r+1)/2 >= n,
 *            k = n - r(r-1)/2, and T(r,k)=1 + k*(r-1)+(2*k - r -1)*(k mod 2).
 *            Equivalently: if k is odd, T = k*(r+1) - r; if k is even, T = 1
 *            + k*(r-1)
 * Range:     u8 n <= 31, u16 n <= 6828, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 94 terms through n = 9901 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; n=0 is not used */

    seq_t two_n = (seq_t)2 * (seq_t)n;
    seq_t r = floor_sqrt(two_n);

    while (r * (r + 1) / 2 < n)
        r++;

    seq_t prev_total = r * (r - 1) / 2;
    seq_t k = (seq_t)n - prev_total;

    if (k & 1)
        return k * (r + 1) - r;
    else
        return 1 + k * (r - 1);
}
