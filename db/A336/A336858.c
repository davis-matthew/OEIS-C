/*
 * A336858  Triangle read by rows: T(n,k) = T(n,k-1) + T(n-1, k) + T(n-1,k-1)
 *          with T(n,0) = T(n, n) = 1 (n >= 0, 0 <= k <= n).
 * https://oeis.org/A336858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n,k-1) + T(n-1,k) + T(n-1,k-1) with T(n,0)=T(n,n)=1;
 *            a(n) = T(row,col) where row = floor((sqrt(1+8n)-1)/2) and col =
 *            n - row(row+1)/2
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-03)
 * Helpers:   isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/isqrt.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* find row (r) and column (c) for index n */
    idx_t d = 1 + (idx_t)8 * n;
    idx_t sqrt_d = isqrt(d);
    idx_t r = (sqrt_d - 1) / 2;          /* initial guess */

    /* adjust r to satisfy r(r+1)/2 <= n < (r+1)(r+2)/2 */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    idx_t col = n - (unsigned __int128)r * (r + 1) / 2;

    /* dynamic construction of the triangle up to row r */
    idx_t max_rows = r + 1;
    seq_t *row   = mem_new(seq_t, max_rows);
    seq_t *next  = mem_new(seq_t, max_rows);

    row[0] = 1;                          /* row 0 */

    if (r > 0) {
        for (idx_t i = 1; i <= r; ++i) {
            next[0] = 1;
            next[i] = 1;
            for (idx_t k = 1; k < i; ++k) {
                next[k] = next[k - 1] + row[k] + row[k - 1];
            }
            /* swap row and next for next iteration */
            seq_t *tmp = row;
            row = next;
            next = tmp;
        }
    }

    seq_t ans = row[col];

    mem_free(row);
    mem_free(next);
    return ans;
}
