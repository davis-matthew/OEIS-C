/*
 * A368522  Triangular array T, read by rows: T(n,k) = number of sums |x-y| +
 *          |y-z| - |x-z| = 2n-2-k, where x,y,z are in {1,2,...,n}.
 * https://oeis.org/A368522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term in the flattened triangular array T(r, k)
 *            = |x-y| + |y-z| - |x-z| = 2(r - k), with r the row (1‑based).
 *            The array is read by rows, and each row is reversed, so the
 *            first element of row r has k = r (t = 0), the last element has k
 *            = 1 (t = r-1). Thus for a given position pos i...
 * Range:     u8 n <= 27, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n   (row_end(r))          */
    idx_t low = 1, high = 2 * n;                 /* high is surely enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                               /* row number */

    /* Compute the starting index of this row */
    idx_t start = r * (r - 1) / 2 + 1;

    /* Position within the row (1‑based, left‑to‑right in reversed array) */
    idx_t pos = n - start + 1;

    if (pos == r) {                              /* e == 0 case */
        seq_t rr = (seq_t)r;
        seq_t rr2 = rr * rr;
        seq_t prod = rr * (rr2 - (seq_t)1);      /* r*(r^2 - 1) */
        seq_t val = prod / 3 + rr2;              /* (r*(r^2-1))/3 + r^2 */
        return val;
    } else {                                     /* e > 0 case */
        seq_t p = (seq_t)pos;
        return 2 * p * p;                        /* 2*pos^2 */
    }
}
