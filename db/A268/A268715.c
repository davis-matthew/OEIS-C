/*
 * A268715  Square array A(i,j) = A003188(A006068(i) + A006068(j)), read by
 *          antidiagonals as A(0,0), A(0,1), A(1,0), A(0,2), A(1,1), A(2,0),
 *          ...
 * https://oeis.org/A268715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(i,j) = A003188(A006068(i) + A006068(j)), read by
 *            antidiagonals; with n = d(d+1)/2 + i, j = d - i: a(n) = A(i,j)
 * Range:     u8 n <= 8385, u16 n <= 15050, u32 n <= 15050, u64 n <= 15050,
 *            u128 n <= 15050, big any n
 * Validated: standard, pass, 121 terms through n = 9639 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A006068_inv_gray(idx_t n)
{
    seq_t result = (seq_t)n;
    seq_t shifted = (seq_t)n;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

static seq_t A003188_gray(seq_t x)
{
    return x ^ (x >> 1);
}

seq_t a(idx_t n)
{
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * (mid + 1) / 2 <= (uint64_t)n) lo = mid;
        else hi = mid - 1;
    }
    uint64_t d = lo;
    idx_t i = n - (idx_t)(d * (d + 1) / 2);
    idx_t j = (idx_t)d - i;
    seq_t s = A006068_inv_gray(i) + A006068_inv_gray(j);
    return A003188_gray(s);
}
