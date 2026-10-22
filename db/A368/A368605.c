/*
 * A368605  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y| + |y-z| = k, where x,y,z are in {1,2,...,n} and x < y
 *          and y >= z.
 * https://oeis.org/A368605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r+1, k), where - r is the row number such that r(r+1) ≥
 *            n and (r-1)r < n - k = n - (r-1)r (1 ≤ k ≤ 2r) - T(m,k) is the
 *            number of triples (x,y,z) with 1 ≤ x < y ≤ m, 1 ≤ z ≤ y and
 *            |x-y| + |y-z| = k
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑free binary search to find row r */
static inline idx_t row_of(idx_t pos)
{
    idx_t lo = 0;
    /* hi can be set arbitrarily large; 0xFFFFFFFF (≈4·10⁹) is above
       √(max(idx_t)) for 64‑bit types. */
    idx_t hi = 0xFFFFFFFFULL;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* Use 128‑bit to avoid overflow when computing mid*(mid+1) */
        __int128 prod = (__int128)mid * (mid + 1);
        if (prod >= pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Count T(m,k) without enumerating triples */
static inline seq_t count_T(idx_t m, idx_t k)
{
    seq_t total = 0;
    for (idx_t y = 2; y <= m; ++y) {
        /* Compute lower and upper bounds for x as derived:
           max(1, y-k) ≤ x ≤ min(y-1, 2y-k-1) */
        int64_t y64   = (int64_t)y;
        int64_t k64   = (int64_t)k;
        int64_t low   = (y64 > k64) ? (y64 - k64) : 1;
        if (low < 1) low = 1;
        int64_t high  = y64 - 1;
        int64_t cand  = 2 * y64 - k64 - 1;
        if (cand < high) high = cand;
        if (high >= low) {
            total += (seq_t)(high - low + 1);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* just to be safe, though offset is 1 */
    /* Determine row r and offset k in that row */
    idx_t r = row_of(n);                      /* r ≥ 1 */
    __int128 prev_sum128 = (__int128)(r - 1) * r;
    idx_t prev_sum = (idx_t)prev_sum128;
    idx_t k = n - prev_sum;           /* 1 ≤ k ≤ 2r */
    idx_t m = r + 1;                  /* T index is (row+1) */
    return count_T(m, k);
}
