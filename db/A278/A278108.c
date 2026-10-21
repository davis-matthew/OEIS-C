/*
 * A278108  Irregular triangle read by rows: T(n,k) = floor(n/k^2) for 1 <=
 *          k^2 <= n.
 * https://oeis.org/A278108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2600, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A278108:  Irregular triangle read by rows
 *  T(n,k) = floor(n / k^2)   for 1 ≤ k^2 ≤ n
 *  a(n) is the n‑th element when the triangle is read row by row (1‑based).
 *
 *  Implementation approach:
 *  1. Find the row r that contains the n‑th element by accumulating the row
 *     lengths floor(sqrt(i)).  The row length is obtained by an integer
 *     square‑root routine written in terms of idx_t only.
 *  2. Compute the column index within that row.
 *  3. Return floor(r / k^2) using seq_t arithmetic.
 */

#include <stdint.h>   /* for uint64_t (already supplied via build) */

/* Integer floor square root for idx_t.  Works for all values that fit
 * in idx_t; the multiplication mid*mid is safe because mid ≤ sqrt(x). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to prevent infinite loop */
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the a(n) of OEIS A278108.  */
seq_t a(idx_t n)
{
    /* 1. Locate the row r containing the n-th term. */
    idx_t r   = 1;        /* row index (1‑based) */
    idx_t cum = 0;        /* total terms in rows before r */

    while (1)
    {
        idx_t row_len = isqrt_u64(r);      /* floor(sqrt(r)) */
        if (cum + row_len >= n)                   /* r is the desired row */
            break;
        cum += row_len;
        ++r;
    }

    /* 2. Column position in this row (1‑based). */
    idx_t offset = n - cum - 1;   /* 0‑based offset in the row */
    idx_t k      = offset + 1;    /* column index */

    /* 3. Compute floor(r / k^2). */
    seq_t rn = (seq_t)r;
    seq_t kk = (seq_t)k * (seq_t)k;   /* k^2 fits into seq_t safely for n ≤ 10⁴ */
    return rn / kk;
}
