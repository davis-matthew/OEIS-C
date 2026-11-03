/*
 * A371835  Triangle read by rows: T(n,k) is the number of points (x,y,z)
 *          satisfying |x|+|y|+|z|<=n and max(|x|,|y|,|z|)<=k; 0<=k<=n.
 * https://oeis.org/A371835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle T(n,k) read * by rows,
 *            where T(n,k) counts lattice points (x,y,z) with * |x|+|y|+|z| ≤
 *            n and max(|x|,|y|,|z|) ≤ k, 0 ≤ k ≤ n. * a(0) is the first term
 *            of the triangle
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Helper: count of integer pairs (y,z) with
 * |y|+|z| ≤ r and |y|,|z| ≤ k.
 */
static inline seq_t G2(idx_t k, idx_t r)
{
    if (r == (idx_t)-1) return 0;  /* unreachable, but keeps compiler sane */
    if (r == 0) {
        /* only (0,0) */
        return 1;
    }

    /* m0 = min(k, r) */
    idx_t m0_idx = (k < r) ? k : r;
    seq_t m0 = (seq_t)m0_idx;

    /* base count from single non‑zero coordinates */
    seq_t result = 1 + 4 * m0;  /* (0,0) + 4 per unit in a single axis */

    /* count of pairs with both coordinates non‑zero */
    seq_t countPairs = 0;
    if (r >= 2) {
        if (r <= k + 1) {
            /* Classic triangle count */
            seq_t sm = (seq_t)r;
            countPairs = (sm - 1) * sm / 2;
        } else if (r < 2 * k) {
            seq_t d   = (seq_t)(r - k);   /* 1 ≤ d ≤ k-1 */
            seq_t len = (seq_t)(2 * k - r); /* 1 ≤ len ≤ k-1 */
            seq_t sm  = (seq_t)r;
            countPairs = (seq_t)k * d + len * (sm - 1) / 2;
        } else {
            /* r ≥ 2k: all pairs bounded by the square */
            countPairs = (seq_t)k * (seq_t)k;
        }
    }

    result += 4 * countPairs;
    return result;
}

/* 3‑dimensional count: T(n,k) */
static inline seq_t G3(idx_t k, idx_t n)
{
    /* a = 0 gives all pairs with sum ≤ n */
    seq_t res = G2(k, n);

    /* a ≥ 1 */
    for (idx_t a = 1; a <= k && a <= n; ++a) {
        res += 2 * G2(k, n - a);  /* 2 signs for the x‑coordinate */
    }
    return res;
}

/* Find row r and column k such that
 * row r has r+1 entries and cumulative count up to row r-1 is
 * r*(r+1)/2 <= n < (r+1)*(r+2)/2.
 */
static inline void row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Simple linear search is fine for n ≤ 10⁴,
     * but we keep it generic using binary search. */
    idx_t lo = 0, hi = n * 2 + 2;  /* upper bound is safe */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* compute mid*(mid+1)/2 safely */
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index such that tri > n, so row = lo-1 */
    *row = lo - 1;
    *col = n - (*row * (*row + 1)) / 2;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    row_col_from_index(n, &r, &k);
    return G3(k, r);
}
