/*
 * A352499  Irregular triangle read by rows: T(n,k) is the sum of all parts of
 *          the partition of n into consecutive parts that contains 2*k-1
 *          parts, and the first element of the column k is in row A000384(k).
 * https://oeis.org/A352499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the unique row index such that
 *            ∑_{i=1}^{r-1} L(i) < n ≤ ∑_{i=1}^{r} L(i) L(i) =
 *            ⌊(√(8i+1)+1)/4⌋, k = n – ∑_{i=1}^{r-1} L(i), and T(r,k) = r if
 *            (2k‑1) divides r, otherwise 0
 * Range:     u8 n <= 1870, u16 n <= 9495, u32 n <= 9495, u64 n <= 9495,
 *            u128 n <= 9495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static inline idx_t row_length(idx_t r)
{
    /* L(r) = ⌊(√(8r+1)+1)/4⌋ */
    idx_t s = floor_sqrt_u64(8ULL * r + 1ULL);
    return (s + 1) >> 2;   /* divide by 4 */
}

seq_t a(idx_t n)
{
    idx_t cum = 0;   /* number of terms already accounted for */
    idx_t r   = 1;   /* current row */

    /* Find the row that contains the n‑th term. */
    while (1) {
        idx_t len = row_length(r);
        if (cum + len >= n)
            break;
        cum += len;
        ++r;
    }

    /* Position within the row. */
    idx_t offset = n - cum - 1;          /* 0‑based */
    idx_t delta  = (offset << 1) + 1;    /* 2*offset + 1 = 2k‑1 */

    /* T(r,k) = r if delta divides r, else 0. */
    return (r % delta == 0) ? (seq_t)r : (seq_t)0;
}
