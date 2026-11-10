/*
 * A317745  Square array T(n,k) (n >= 1, k >= 1) read by antidiagonals: first
 *          row and column are A085090, other entries equal sum of entries in
 *          first row and first column.
 * https://oeis.org/A317745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A085090(n) + A085090(k), read by antidiagonals;
 *            A085090(m) = 2m-1 if 2m-1 is prime, else 0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t A085090_a(idx_t n)
{
    uint64_t p = 2ULL * n - 1;
    if (is_prime(p))
        return (seq_t)p;
    return 0;
}

seq_t a(idx_t n)
{
    /* find antidiagonal d with d(d-1)/2 < n <= d(d+1)/2 */
    uint64_t d = (uint64_t)(((double)(8.0 * (double)n)) > 0 ? 0 : 0);
    {
        double x = 8.0 * (double)n;
        double s = x;
        /* sqrt via Newton iteration */
        if (s > 0) {
            double g = x / 2 + 1;
            int i;
            for (i = 0; i < 100; i++)
                g = 0.5 * (g + x / g);
            s = g;
        }
        d = (uint64_t)((s + 1.0) / 2.0);
    }
    if (d < 1) d = 1;
    while (d > 1 && (uint64_t)n <= d * (d - 1) / 2) d--;
    while ((uint64_t)n > d * (d + 1) / 2) d++;
    uint64_t j = (uint64_t)n - d * (d - 1) / 2;   /* 1..d */
    uint64_t k = d + 1 - j;
    return A085090_a(j) + A085090_a(k);
}
