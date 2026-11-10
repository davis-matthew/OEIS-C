/*
 * A371766  Triangle read by rows: T(n, k) = A371898(n, k) / A371767(n, k).
 * https://oeis.org/A371766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k-1) + k*(n-k)*T(n-1,k), T(n,0) = 1, T(n,k) = 0
 *            for k > n; a(n) is the triangle read by rows
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((idx_t)((r + 1) * (r + 2) / 2) <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;

    seq_t *row = mem_new(seq_t, (size_t)(r + 2));
    row[0] = 1;
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            seq_t prev_same = (j <= i - 1) ? row[j] : (seq_t)0;
            row[j] = row[j - 1] + (seq_t)j * (seq_t)(i - j) * prev_same;
        }
    }
    seq_t res = row[k];
    mem_free(row);
    return res;
}
