/*
 * A308907  Sum of the fifth largest parts in the partitions of n into 6
 *          squarefree parts.
 * https://oeis.org/A308907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the second smallest part in all partitions of
 *            n into 6 square‑free parts. The implementation counts, for each
 *            possible second smallest value m, the number of non‑decreasing
 *            6‑tuples (p1,m,p3,p4,p5,p6) of square‑free integers with
 *            p1≤m≤p3≤…≤p6 and p1+p3+…+p6 = n. For a fixed m t...
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* ---------- build list of square‑free numbers up to n ---------- */
    size_t N = (size_t)n;               /* maximal sum */
    size_t max = N;                      /* same as N */
    idx_t *sf = (idx_t*)malloc((max + 1) * sizeof(idx_t));
    size_t sf_len = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (mobius(i) != 0) {
            sf[sf_len++] = i;
        }
    }

    /* ---------- dynamic programming for multisets of size ≤4 ---------- */
    size_t cols = N + 1;                 /* number of possible sums 0…N   */
    size_t rows = 5;                     /* k = 0…4  (size of multiset)   */
    seq_t *ways = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    if (ways == NULL) {
        /* memory allocation failed – return 0 as a safe default */
        free(sf);
        return (seq_t)0;
    }
    ways[0] = (seq_t)1;                  /* ways[0][0] = 1 */

    seq_t result = (seq_t)0;

    /* ---------- iterate over possible second smallest values (m = v) ---------- */
    for (long long idx = (long long)sf_len - 1; idx >= 0; --idx) {
        idx_t v = sf[(size_t)idx];

        /* add the new element v to the suffix set: update DP */
        for (size_t k = 1; k <= 4; ++k) {
            size_t base   = k   * cols;
            size_t basePr = (k-1) * cols;
            for (size_t s = v; s <= N; ++s) {
                ways[base + s] += ways[basePr + s - v];
            }
        }

        /* contribution when the second smallest part equals v */
        seq_t *ways4 = ways + 4 * cols;          /* ways[4][*]  */
        for (size_t j = 0; j <= (size_t)idx; ++j) {
            idx_t p1 = sf[j];
            if (v + p1 > n) continue;           /* remaining would be negative */
            idx_t remaining = n - v - p1;
            result += (seq_t)v * ways4[remaining];
        }
    }

    free(sf);
    free(ways);
    return result;
}
