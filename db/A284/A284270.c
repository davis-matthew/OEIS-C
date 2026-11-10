/*
 * A284270  Square array A(r,c) = A048720(A065621(r), c) mod r, read by
 *          descending antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3),
 *          A(2,2), A(3,1), etc.
 * https://oeis.org/A284270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   A(r,c) = A048720(A065621(r), c) mod r, where A048720(x,c) is the
 *            carryless (GF(2)[X]) product x*c and A065621(r) = r XOR ((r -
 *            lowbit(r)) << 1); read by descending antidiagonals, a(n) =
 *            A(r,c) with n = d(d-1)/2 + r, c = d+1-r
 * Range:     u8 n <= 398, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 136 terms through n = 9988 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A284270_rev(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t low_t = (seq_t)(n & (~n + 1));
    return n_t ^ ((n_t - low_t) << 1);
}

static seq_t A284270_gf2_mul(seq_t x, idx_t j)
{
    seq_t result = 0;
    seq_t shifted = x;
    while (j) {
        if (j & 1)
            result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* smallest d with d(d+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((idx_t)(((unsigned __int128)hi * (hi + 1)) / 2) < n)
        hi *= 2;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((idx_t)(((unsigned __int128)mid * (mid + 1)) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;
    idx_t k = n - (idx_t)(((unsigned __int128)d * (d - 1)) / 2); /* 1..d */
    idx_t r = k;
    idx_t c = d + 1 - k;
    seq_t p = A284270_gf2_mul(A284270_rev(r), c);
    return p % (seq_t)r;
}
