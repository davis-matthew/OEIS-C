/*
 * A260910  Triangle read by rows: Fresenius numbers of n and A077664(n,k), k
 *          = 1..n.
 * https://oeis.org/A260910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing index n * and T(r,k)
 *            = (r-1) * A(r,k) - r. * A(r,k) is the k-th integer > r that is
 *            coprime to r
 * Range:     i8 n <= 34, i16 n <= 4079, i32 n <= 7875, i64 n <= 7875,
 *            i128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute the row number r for a given triangle index n.
 * The minimal r such that r*(r+1)/2 >= n. */
static inline idx_t row_for_index(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* exponential search to find an upper bound */
    while (((__int128)hi * (hi + 1)) / 2 < n)
        hi <<= 1;

    /* binary search between lo and hi */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Euclidean gcd for idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t r = row_for_index(n);

    /* Number of elements before row r: (r-1) * r / 2 */
    idx_t prev = ((__int128)(r - 1) * r) / 2;
    /* Position k inside the row (1‑based) */
    idx_t k = n - prev;

    /* Find the k‑th integer > r that is coprime to r */
    idx_t count = 0;
    idx_t m = r + 1;
    idx_t A_val = 0;
    while (count < k) {
        if (gcd_idx(m, r) == 1) {
            ++count;
            if (count == k)
                A_val = m;
        }
        ++m;
    }

    /* Compute T(r,k) = (r-1) * A_val - r */
    __int128 prod = ((__int128)(r - 1)) * ((__int128)A_val);
    seq_t result = (seq_t)(prod - (__int128)r);
    return result;
}
