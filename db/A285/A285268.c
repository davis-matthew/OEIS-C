/*
 * A285268  Triangle read by rows: T(m,n) = Sum_{i=1..n} P(m,i) where P(m,n) =
 *          m!/(m-n)! is the number of permutations of m items taken n at a
 *          time, for 1 <= n <= m.
 * https://oeis.org/A285268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where m = the row index satisfying (m-1)*m/2 < n ≤
 *            m*(m+1)/2 k = n - (m-1)*m/2 (the column index, 1 ≤ k ≤ m) T(m,k)
 *            = Σ_{i=1..k} P(m,i) with P(m,i) = m!/(m-i)!
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 88, u64 n <= 228,
 *            u128 n <= 593, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for uint64_t x */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n) {
    /* find row m */
    idx_t m = (isqrt_u64(8*n + 1) - 1) / 2;
    if ((m * (m + 1)) / 2 < n) m++;

    /* column index k */
    idx_t prev = (m * (m - 1)) / 2;
    idx_t k = n - prev;   /* 1‑based within the row */

    seq_t sum = 0;
    seq_t perm = 1;
    for (idx_t i = 1; i <= k; ++i) {
        perm = perm * (seq_t)(m - (i - 1));
        sum += perm;
    }
    return sum;
}
