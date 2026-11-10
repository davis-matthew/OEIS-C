/*
 * A378198  Table T(n, k) read by upward antidiagonals. T(n,1) = A375602(n),
 *          T(n,2) = A375602(A375602(n)), T(n,3) =
 *          A375602(A375602(A375602(n))) and so on.
 * https://oeis.org/A378198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A375602 applied k times to n, read by upward
 *            antidiagonals: a(m) = T(d-i+1, i) where d(d-1)/2 < m <=
 *            d(d+1)/2, i = m - d(d-1)/2
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 130 terms through n = 8823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A375602_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t A375602_a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t t  = A375602_isqrt_u64(8 * n + 1);
    idx_t d  = (t - 1) / 2;
    while (d * (d + 1) / 2 < n)
        ++d;
    idx_t Ld = d;
    idx_t Rd = n - (Ld - 1) * Ld / 2;

    idx_t cum  = 0;
    idx_t len  = 0;
    idx_t L    = 0;
    for (L = 1; ; ++L) {
        len = L * (L * L + 1) / 2;
        cum += len;
        if (n <= cum)
            break;
    }

    seq_t P = (seq_t)L * Rd + Ld - (seq_t)L * (L + 1) / 2;

    long long over = (long long)Rd - ((long long)L * L - L + 2) / 2;
    if (over < 0) over = 0;
    P -= (seq_t)over * (over + 1) / 2;

    seq_t cumPrev = (seq_t)(L - 1) * L * ((seq_t)L * L - L + 2) / 8;

    return P + cumPrev;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;
    idx_t i = n - d * (d - 1) / 2;   /* k, 1..d */
    idx_t x = d - i + 1;             /* row */
    idx_t j;
    for (j = 0; j < i; ++j)
        x = (idx_t)(unsigned long long)A375602_a(x);
    return (seq_t)x;
}
