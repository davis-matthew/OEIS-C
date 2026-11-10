/*
 * A349780  Triangle read by rows, T(n, k) = A000262(n) - A349776(n, n - k)
 *          for n > 0 and T(0, 0) = 1.
 * https://oeis.org/A349780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=n-k+1..n} binomial(n-1,j-1)*n!/j! for n > 0,
 *            T(0,0) = 1 (equivalently A000262(n) - A349776(n,n-k)); a(n) is
 *            the triangle read by rows
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0, k, i, m, j;
    seq_t *row;
    seq_t p, sum;

    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    k = n - r * (r + 1) / 2;

    if (r == 0)
        return (seq_t)1;

    /* Pascal row r-1 (additions only, exact) */
    row = mem_new(seq_t, r + 1);
    row[0] = 1;
    for (i = 1; i < r; i++)
        for (m = i; m >= 1; m--)
            row[m] += row[m - 1];

    sum = 0;
    p = 1; /* r!/j! */
    for (j = r; j > r - k; j--) {
        if (j < r)
            p *= (seq_t)(j + 1);
        sum += row[j - 1] * p;
    }
    mem_free(row);
    return sum;
}
