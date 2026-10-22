/*
 * A350898  Number of partitions of n such that (smallest part) = 4*(number of
 *          parts).
 * https://oeis.org/A350898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that smallest part =
 *            4*(number of parts)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, ... (OEIS A350898)   */
/*  Implementation: for each k>=1 with 4k² ≤ n, add the number of partitions of n−4k² into at most k–1 parts.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"   /* alloc_seq(size_t) */
#include "../utils/free_seq.h"    /* free_seq(seq_t *) */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* find maximal k with 4k^2 <= n */
    idx_t kmax = 0;
    for (idx_t k = 1;; ++k) {
        unsigned long long val = 4ull * (unsigned long long)k * (unsigned long long)k;
        if (val > n) break;
        kmax = k;
    }

    /* DP table: part[t][m] = partitions of m into at most t parts
       t ranges 0..kmax (inclusive, row 0 is base).  */
    size_t rows = (size_t)(kmax + 1);
    size_t cols = (size_t)(n + 1);
    size_t total = rows * cols;
    seq_t *part = alloc_seq(total);                /* part[t*cols + m] */

    /* initialize row 0 */
    for (size_t m = 0; m < cols; ++m)
        part[m] = (m == 0) ? 1 : 0;

    /* fill rows 1 .. kmax */
    for (size_t t = 1; t < rows; ++t) {
        size_t base_t    = t * cols;
        size_t base_prev = (t - 1) * cols;
        part[base_t] = 1;                 /* m==0 */
        for (size_t m = 1; m < cols; ++m) {
            seq_t val = part[base_prev + m];
            if (m >= t) val += part[base_t + (m - t)];
            part[base_t + m] = val;
        }
    }

    /* accumulate answer */
    seq_t res = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        size_t m = (size_t)(n - (unsigned long long)4 * k * k);
        size_t t = (size_t)(k - 1);
        res += part[t * cols + m];
    }

    free_seq(part);
    return res;
}
