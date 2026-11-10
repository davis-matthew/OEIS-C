/*
 * A391245  Triangular table T(n,k) = n*k if A048720(n, A065621(k)) = n*k,
 *          otherwise 0, read by rows for n >= 1, 1 <= k <= n.
 * https://oeis.org/A391245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = n*k if A048720(n, A065621(k)) = n*k, else 0, read by
 *            rows (1<=k<=n); A065621(k) = k XOR ((k - lowbit(k)) << 1),
 *            A048720 = carryless product
 * Range:     u8 n <= 135, u16 n <= 10440, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 107 terms through n = 9323 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t T_a065621(idx_t k)
{
    seq_t kt = (seq_t)k;
    seq_t low = kt & (seq_t)(((idx_t)0) - k);
    return kt ^ ((kt - low) << 1);
}

static seq_t T_clmul(seq_t x, seq_t y)
{
    seq_t r = 0;
    while (y != 0) {
        if ((y & 1) != 0) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* find row r: r(r-1)/2 < n <= r(r+1)/2 */
    idx_t lo = 1, hi = 6000000000ULL;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;
    idx_t k = n - r * (r - 1) / 2;
    seq_t prod = (seq_t)r * (seq_t)k;
    seq_t cl = T_clmul((seq_t)r, T_a065621(k));
    return (cl == prod) ? prod : (seq_t)0;
}
