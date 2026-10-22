/*
 * A330613  Triangle read by rows: T(n, k) = 1 + k - 2*n - 2*k*n + 2*n^2, with
 *          0 <= k < n.
 * https://oeis.org/A330613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 + k - 2*r - 2*k*r + 2*r^2, where r = row index, * k =
 *            column index within that row, computed from the linear index n *
 *            with the triangle read row by row (0‑based columns). r is found
 *            by * solving r*(r+1)/2 >= n, and k = n - r*(r-1)/2 - 1. This
 *            implements * OEIS A330613
 * Range:     u8 n <= 66, u16 n <= 8792, u32 n <= 8792, u64 n <= 8792,
 *            u128 n <= 8792, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the n-th term of OEIS A330613.
 * n is 1‑based (a(1) = 1). */
seq_t a(idx_t n)
{
    idx_t r = 1;        /* current row, 1‑based */
    idx_t sum = 1;      /* total terms up to current row */

    /* Find the row r such that sum_{i=1}^{r} i >= n. */
    while (sum < n) {
        r++;
        sum += r;
    }

    /* Previous cumulative count before row r. */
    idx_t prev_sum = sum - r;
    /* Offset within the row (1‑based) and column index k. */
    idx_t k = n - prev_sum - 1;   /* 0‑based column */

    /* Perform the formula in a wider unsigned type to avoid
     * intermediate overflow on small seq_t sizes. */
    uint64_t r64 = (uint64_t)r;
    uint64_t k64 = (uint64_t)k;
    uint64_t temp = 2ULL * r64 * r64;          /* 2*r^2 */
    temp -= 2ULL * k64 * r64;                  /* -2*k*r */
    temp -= 2ULL * r64;                        /* -2*r */
    temp += k64;                               /* +k */
    temp += 1ULL;                              /* +1 */

    return (seq_t)temp;
}
