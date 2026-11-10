/*
 * A277320  Square array A(n,k) = A048720(A065621(n), k), read by descending
 *          antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1),
 *          etc.
 * https://oeis.org/A277320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   A(r,c) = A048720(A065621(r), c), the carryless (GF(2)[X])
 *            product of A065621(r) = r XOR ((r - lowbit(r)) << 1) and c; the
 *            array is read by descending antidiagonals
 * Range:     u8 n <= 284, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 107 terms through n = 8740 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gf2_mul_277320(seq_t x, idx_t y)
{
    seq_t result = 0;
    seq_t shifted = x;
    while (y) {
        if (y & 1)
            result ^= shifted;
        shifted <<= 1;
        y >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find d with d(d-1)/2 < n <= d(d+1)/2 */
    idx_t lo = 1, hi = 6200000000ULL;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;
    idx_t before = (idx_t)(((unsigned __int128)d * (d - 1)) / 2);
    idx_t r = n - before;        /* row, 1..d */
    idx_t c = d + 1 - r;         /* column */

    idx_t low = r & (~r + 1);
    seq_t rv = (seq_t)r ^ (((seq_t)r - (seq_t)low) << 1);
    return gf2_mul_277320(rv, c);
}
