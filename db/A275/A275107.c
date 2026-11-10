/*
 * A275107  Limiting sequence of the least significant digits of the
 *          odd-indexed terms of A027878 in reverse order.
 * https://oeis.org/A275107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digit n-1 (from the right, 0-indexed) of the 10-adic
 *            limit of A027878(m) for odd m, i.e. of (-1)^n *
 *            Product_{i=1..n-1} (10^i - 1) mod 10^n; computed with a decimal
 *            digit array
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Validated: standard, pass, 96 terms through n = 294 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    int *r = (int *)malloc(sizeof(int) * N);
    int *t = (int *)malloc(sizeof(int) * N);
    size_t i, j;
    for (j = 0; j < N; ++j) r[j] = 0;
    r[0] = 1;

    for (i = 1; i + 1 <= N; ++i) {
        /* t = (r * 10^i - r) mod 10^N */
        int borrow = 0;
        for (j = 0; j < N; ++j) {
            int sh = (j >= i) ? r[j - i] : 0;
            int d = sh - r[j] - borrow;
            if (d < 0) { d += 10; borrow = 1; } else borrow = 0;
            t[j] = d;
        }
        int *tmp = r; r = t; t = tmp;
    }

    if (N & 1) {
        /* negate mod 10^N */
        int borrow = 0;
        for (j = 0; j < N; ++j) {
            int d = 0 - r[j] - borrow;
            if (d < 0) { d += 10; borrow = 1; } else borrow = 0;
            r[j] = d;
        }
    }

    int res = r[N - 1];
    free(r);
    free(t);
    return (seq_t)res;
}
