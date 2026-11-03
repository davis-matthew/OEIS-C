/*
 * A285732  Square array A(n,k) read by antidiagonals, A(n,n) = -n, otherwise,
 *          if n > k, A(n,k) = T(n-k,k), else A(n,k) = T(n,k-n), where T(n,k)
 *          is sequence A000027 considered as a two-dimensional table.
 * https://oeis.org/A285732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) computed by navigating the square array A285732 enumerated
 *            by antidiagonals. For each n ≥ 1 find the cell (row,col) that
 *            contains the n-th element, then: -row if row == col T(row‑col,
 *            col) if row > col T(row, col‑row) if row < col where T(u,v) =
 *            (u+v‑2)*(u+v‑1)/2 + u
 * Range:     i8 n <= 152, i16 n <= 10585, i32 n <= 10585, i64 n <= 10585,
 *            i128 n <= 10585, big any n
 * Validated: standard, pass, 107 terms through n = 9699 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t triangular_level(idx_t n)
{
    /* Find the smallest d such that d*(d+1)/2 ≥ n. */
    idx_t lo = 0;
    idx_t hi = 1;

    /* Grow hi until it covers n. */
    while (true) {
        __int128 tri = (__int128)hi * (hi + 1) / 2;
        if (tri >= n || hi == UINT64_MAX) break;
        hi <<= 1;                     /* hi *= 2 */
        if (hi == 0) {                /* overflow recovered */
            hi = UINT64_MAX;
            break;
        }
    }

    /* Binary search to find minimal d with d*(d+1)/2 ≥ n. */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;
}

static inline seq_t cantor_pair(idx_t n, idx_t k)
{
    /* Cantor pairing for 1‑based indices (n,k ≥1). */
    __int128 s = (__int128)(n + k - 2);
    __int128 t = (__int128)(n + k - 1);
    __int128 val = s * t / 2 + n;
    return (seq_t)val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, safe fallback for offset 0 */

    // Locate the antidiagonal containing the n‑th element
    idx_t d = triangular_level(n);

    // Number of elements before this diagonal: (d-1)*d/2
    __int128 prev = (__int128)(d - 1) * d / 2;
    __int128 diff = (__int128)n - prev - 1;  /* 0‑based offset */
    idx_t offset = (idx_t)diff;

    // Coordinates on the diagonal (row, col) using 1‑based indices
    idx_t row = offset + 1;          /* n */
    idx_t col = d - offset;          /* k */

    // Compute A(row, col) according to the definition
    if (row == col) {
        return -(seq_t)row;
    } else if (row > col) {
        return cantor_pair(row - col, col);
    } else { /* row < col */
        return cantor_pair(row, col - row);
    }
}
