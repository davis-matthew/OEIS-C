/*
 * A350637  Triangle read by rows: T(n,k) in which row n lists the first n
 *          terms of A024916 in reverse order, 1 <= k <= n.
 * https://oeis.org/A350637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A024916(n-k+1) = Sum_{j=1..n-k+1} j*floor((n-k+1)/j);
 *            a(N) is the N-th entry of the triangle read by rows, row r =
 *            ceil((sqrt(8N+1)-1)/2), k = N - r(r-1)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A350637_sumdiv(idx_t m)
{
    /* A024916(m) = Sum_{k=1..m} k*floor(m/k), grouped by equal quotients */
    seq_t res = 0;
    idx_t k = 1;
    while (k <= m) {
        idx_t q = m / k;
        idx_t hi = m / q;
        seq_t cnt = (seq_t)(hi - k + 1);
        seq_t s = ((seq_t)k + (seq_t)hi) * cnt / 2;
        res += s * (seq_t)q;
        k = hi + 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row r: smallest r with r(r+1)/2 >= n */
    idx_t r = 0;
    {
        idx_t lo = 1, hi = 6000000000ULL;
        while (lo < hi) {
            idx_t mid = lo + (hi - lo) / 2;
            unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
            if (t >= (unsigned __int128)n) hi = mid;
            else lo = mid + 1;
        }
        r = lo;
    }
    idx_t prev = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t k = n - prev;          /* 1 <= k <= r */
    idx_t m = r - k + 1;
    return A350637_sumdiv(m);
}
