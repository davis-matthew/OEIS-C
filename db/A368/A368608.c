/*
 * A368608  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y| + |y-z| = k, where x,y,z are in {1,2,...,n} and x != y
 *          and y <= z.
 * https://oeis.org/A368608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) lists the irregular triangular array T. T(n,k) = # of
 *            triples (x,y,z) in {0,…,n} with (i) x ≠ y , (ii) y ≤ z , (iii)
 *            |x−y| + |y−z| = k , where 1 ≤ k ≤ 2n. Row n has 2n terms, for k
 *            = 1,…,2n. a(n) is the value at the n‑th position of this
 *            flattened list
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count T(n,k) for a given row size n (≥1) and index k (1…2n). */
static seq_t count_T(idx_t n, idx_t k)
{
    seq_t cnt = 0;

    for (idx_t y = 0; y <= n; ++y) {
        idx_t max_d2 = n - y;                  /* z−y max */
        for (idx_t d1 = 1; d1 <= n; ++d1) {    /* |x−y| > 0 */
            if (k < d1)                     /* d2 would be negative */
                continue;
            idx_t d2 = k - d1;
            if (d2 > max_d2)                /* z would exceed n */
                continue;

            seq_t add = 0;
            if (y + d1 <= n)
                add++;                      /* x = y + d1 */
            if (y >= d1)
                add++;                      /* x = y – d1 */
            cnt += add;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find row r such that (r−1)r < n ≤ r(r+1). */
    idx_t r = 1;
    while (r * (r + 1) < n)
        ++r;

    /* First index of this row and column number k (1‑based). */
    idx_t first_in_row = (r - 1) * r + 1;
    idx_t k = n - first_in_row + 1;

    return count_T(r, k);
}
