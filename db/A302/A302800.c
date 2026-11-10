/*
 * A302800  Irregular triangle read by rows: T(n,k) is the area of the k-th
 *          region of the diagram with n rows described in A237591.
 * https://oeis.org/A302800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Sum_{m=1..n} (C(m,k) - C(m,k+1)), where C(m,k) =
 *            floor((m-k(k-1)/2)/k) if m >= k(k+1)/2, else 0 (C(m,k) = number
 *            of partitions of integers <= m into exactly k consecutive
 *            parts); row n has A003056(n) terms
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t a302800_len(idx_t r)
{
    /* largest L with L(L+1)/2 <= r */
    idx_t L = 0;
    idx_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        if (mid * (mid + 1) / 2 <= r) lo = mid; else hi = mid - 1;
    }
    L = lo;
    return L;
}

static idx_t a302800_C(idx_t m, idx_t k)
{
    if (m < k * (k + 1) / 2) return 0;
    return (m - k * (k - 1) / 2) / k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = 0, cum = 0;
    while (1) {
        ++r;
        idx_t len = a302800_len(r);
        if (cum + len >= n) {
            idx_t k = n - cum;
            seq_t s = 0;
            for (idx_t m = 1; m <= r; ++m) {
                idx_t c1 = a302800_C(m, k);
                idx_t c2 = a302800_C(m, k + 1);
                s += (seq_t)(c1 - c2);
            }
            return s;
        }
        cum += len;
    }
}
