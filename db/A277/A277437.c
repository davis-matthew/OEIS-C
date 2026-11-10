/*
 * A277437  Square array read by antidiagonals upwards in which T(n,k) is the
 *          n-th number j such that, descending by the main diagonal of the
 *          pyramid described in A245092, the height difference between the
 *          level j (starting from the top) and the level of the next terrace
 *          is equal to k.
 * https://oeis.org/A277437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   T(n,k) = n-th j such that (next term of A071562 greater than j)
 *            - j = k; array read by antidiagonals upwards (sieve search)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;
    idx_t t = n - d * (d - 1) / 2;   /* 1..d */
    idx_t k = t;
    idx_t row = d - t + 1;

    for (size_t L = 1024;; L *= 2) {
        unsigned char *mark = (unsigned char *)calloc(L + 1, 1);
        if (!mark) abort();
        for (size_t x = 1; x * x <= L; x++) {
            for (size_t y = x; y <= 2 * x && x * y <= L; y++)
                mark[x * y] = 1;
        }
        size_t p = 0;
        idx_t count = 0;
        bool found = false;
        size_t res = 0;
        for (size_t m = 1; m <= L; m++) {
            if (!mark[m]) continue;
            if (p != 0 && (size_t)k < m) {
                size_t j = m - (size_t)k;
                if (j >= p) {
                    count++;
                    if (count == row) {
                        found = true;
                        res = j;
                        break;
                    }
                }
            }
            p = m;
        }
        free(mark);
        if (found) return (seq_t)res;
    }
}
