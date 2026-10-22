/*
 * A368487  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^j * binomial(j+k-1,j).
 * https://oeis.org/A368487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A368487: a(n) = Sum_{j=0..m} k^j * binomial(j+k-1, j)
 * where (m,k) are obtained by unfolding the index n along antidiagonals.
 * For an index n, let d = max{t | t(t+1)/2 <= n} (the diagonal number).
 * Then m = n - d(d+1)/2, k = d - m.  The term is T(m,k) = Σ_{j=0..m} k^j * C(j+k-1,j).
 */

#include <stdint.h>

/* Forward declarations of utility functions that may exist in the build. */
extern idx_t _floor_isqrt_u64(idx_t n);
extern seq_t binomial(idx_t n, idx_t k);

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t low = 0, high = 1;
    /* Expand high until the triangular number exceeds n. */
    while (1) {
        unsigned __int128 tri = (__int128)high * (high + 1) / 2;
        if (tri > n) break;
        high <<= 1;
    }
    /* Binary search for largest d with triangle number <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;

    /* Offset of the first element on diagonal d. */
    unsigned __int128 tri_start = (__int128)d * (d + 1) / 2;
    idx_t offset = (idx_t)tri_start;

    /* Position inside the diagonal. */
    idx_t r = n - offset;          /* r = 0 .. d */
    idx_t m = r;                   /* row index (n in T(n,k)) */
    idx_t k = d - r;               /* column index */

    /* Compute T(m,k) = Σ_{j=0..m} k^j * C(j+k-1, j) */
    seq_t sum = 0;
    seq_t power = 1;               /* k^j */
    seq_t binom = 1;               /* C(j+k-1, j) */
    for (idx_t j = 0; j <= m; ++j) {
        sum += power * binom;
        if (j == m) break;
        /* Update power for next j */
        power *= (seq_t)k;
        /* Update binomial coefficient: C(j+1+k-1, j+1) = C(j+k-1, j) * (j+k) / (j+1) */
        binom = binom * (seq_t)(j + k) / (seq_t)(j + 1);
    }
    return sum;
}
