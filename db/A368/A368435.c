/*
 * A368435  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y|+|y-z| = 2n-2-k, where x,y,z are in {1,...,n}.
 * https://oeis.org/A368435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(m,k) where * m = ceil(sqrt(n)), * k = n - (m-1)^2 - 1
 *            (0‑based index of the row), * T(m,k) = |{(x,y,z) in {1,…,m}³ :
 *            |x‑y| + |y‑z| = 2*m‑2‑k}|
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9942 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* --- count T(n,k) for a given row n and column k --- */
static seq_t compute_T_row(idx_t n, idx_t k)
{
    seq_t total = 0;
    idx_t s = 2 * n - 2 - k;            /* target |x‑y| + |y‑z| */

    for (idx_t y = 1; y <= n; ++y)
    {
        idx_t y_minus1 = y - 1;
        idx_t n_minus_y = n - y;

        for (idx_t a = 0; a <= s; ++a)
        {
            /* count of x with distance a from y */
            idx_t fa_cnt = 0;
            if (a == 0)
                fa_cnt = 1;
            else
            {
                if (a <= y_minus1) ++fa_cnt;
                if (a <= n_minus_y) ++fa_cnt;
            }
            seq_t fa = (seq_t)fa_cnt;

            idx_t b = s - a;              /* distance for z */
            idx_t fb_cnt = 0;
            if (b == 0)
                fb_cnt = 1;
            else
            {
                if (b <= y_minus1) ++fb_cnt;
                if (b <= n_minus_y) ++fb_cnt;
            }
            seq_t fb = (seq_t)fb_cnt;

            total += fa * fb;
        }
    }
    return total;
}

/* --- utility: floor square root for uint64_t --- */
static idx_t floor_sqrt_u64(idx_t v)
{
    idx_t low = 0, high = v, res = 0;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);

        /* avoid division by zero */
        if (mid == 0)
        {
            res = 0;
            low = 1;
            continue;
        }

        if (mid <= v / mid)   /* mid² ≤ v */
        {
            res = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return res;
}

/* --- public sequence function --- */
seq_t a(idx_t n)
{
    /* find the row m such that m² ≥ n (i.e., m = ceil(sqrt(n))) */
    idx_t m = floor_sqrt_u64(n);
    if ((unsigned __int128)m * m < n) ++m;

    /* index within that row (0‑based) */
    idx_t prev_sq = (m > 0) ? (m - 1) * (m - 1) : 0;
    idx_t k = n - prev_sq - 1;   /* 0 ≤ k ≤ 2m‑2 */

    return compute_T_row(m, k);
}
