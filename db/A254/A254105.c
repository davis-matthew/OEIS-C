/*
 * A254105  Dispersion of A055938; starting from its complementary sequence
 *          A005187 as the first column of square array A(row,col), read by
 *          antidiagonals A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1), ...
 * https://oeis.org/A254105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(row,1) = A005187(row) = Sum_{k>=0} floor(row/2^k); A(row,col)
 *            = A055938(A(row,col-1)), read by antidiagonals A(1,1), A(1,2),
 *            A(2,1), ... A055938(m) is the smallest x with x - #{k>=1 :
 *            A005187(k) <= x} >= m, found by binary search
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: standard, pass, 94 terms through n = 120 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t D254105_pop(seq_t k)
{
    seq_t c = 0;
    while (k > 0) {
        c += (k & 1);
        k >>= 1;
    }
    return c;
}

/* f(k) = A005187(k) */
static seq_t D254105_f(seq_t k)
{
    seq_t r = 0;
    while (k > 0) {
        r += k;
        k >>= 1;
    }
    return r;
}

/* number of k >= 1 with A005187(k) <= x */
static seq_t D254105_cnt(seq_t x)
{
    seq_t k = x / 2;
    for (int i = 0; i < 3; i++)
        k = (x + D254105_pop(k)) / 2;
    while (k > 0 && D254105_f(k) > x)
        k--;
    while (D254105_f(k + 1) <= x)
        k++;
    return k;
}

/* number of positive integers <= x not in A005187 */
static seq_t D254105_miss(seq_t x)
{
    return x - D254105_cnt(x);
}

/* A055938(m): smallest x with miss(x) >= m */
static seq_t D254105_comp(seq_t m)
{
    seq_t lo = m;
    seq_t hi = 2 * m + 2;
    while (D254105_miss(hi) < m) {
        lo = hi + 1;
        hi = hi + hi;
    }
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (D254105_miss(mid) >= m)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t d = 1;
    idx_t before = 0; /* terms in diagonals before d */
    while (before + d < n) {
        before += d;
        d++;
    }
    idx_t k = n - before;      /* 1..d */
    idx_t row = k;
    idx_t col = d + 1 - k;
    seq_t v = D254105_f((seq_t)row);
    for (idx_t c = 1; c < col; c++)
        v = D254105_comp(v);
    return v;
}
