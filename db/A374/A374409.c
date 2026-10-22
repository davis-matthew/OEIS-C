/*
 * A374409  Number of ways to write n as an ordered sum of a triangular
 *          number, a pentagonal number and a hexagonal number.
 * https://oeis.org/A374409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (t,p,h) with t = k(k+1)/2
 *            (triangular), p = m(3m-1)/2 (pentagonal), h = l(2l-1)
 *            (hexagonal), and t + p + h = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Mark hexagonal numbers up to n */
    size_t sz = (size_t)n + 1;
    bool *is_hex = (bool *)malloc(sz * sizeof(bool));
    if (!is_hex) return 0;           /* allocation failed */

    for (size_t i = 0; i < sz; ++i) is_hex[i] = false;

    for (idx_t l = 0;; ++l) {
        idx_t h = l * (2 * l - 1);
        if (h > n) break;
        is_hex[(size_t)h] = true;
    }

    seq_t total = 0;

    /* Iterate over triangular numbers */
    idx_t tri = 0;
    for (idx_t k = 0;; ++k) {
        if (tri > n) break;
        idx_t rem1 = n - tri;

        /* Iterate over pentagonal numbers */
        idx_t pent = 0;
        for (idx_t m = 0;; ++m) {
            if (pent > rem1) break;
            idx_t rem2 = rem1 - pent;
            /* rem2 is guaranteed to fit in idx_t */
            if (rem2 <= n && is_hex[(size_t)rem2]) {
                ++total;
            }
            /* pent_{m+1} = pent_m + (3m + 1) */
            pent += 3 * m + 1;
        }

        /* tri_{k+1} = tri_k + (k + 1) */
        tri += k + 1;
    }

    free(is_hex);
    return total;
}
