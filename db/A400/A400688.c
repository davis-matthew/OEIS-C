/*
 * A400688  Triangle of the Catalan Pell numbers read by rows: T(n,k) = [x^n]
 *          (1 - 2*x - Sum_{h=1..k-1} A000108(h)*x^(h+1))^(-1), with 0 < k <=
 *          n.
 * https://oeis.org/A400688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, new
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of the Catalan Pell numbers read by rows.  
 * For n ≥ 1 let r be the smallest integer with r(r+1)/2 ≥ n
 * and let k = n – r(r−1)/2  (1 ≤ k ≤ r).  Then a(n) = T(r,k) where
 *
 *   T(0,k) = 1
 *   T(m,k) = 2·T(m−1,k) + Σ_{h=1}^{k−1} catalan(h)·T(m−h−1,k)   for m ≥ 1
 *
 * This recursive definition is implemented directly below.  The
 * function recomputes the required triangle section for each call.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

#ifdef __cplusplus
#include <vector>
#endif

/* External types defined by the build environment */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* offset starts at 1 */

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    idx_t prev_sum   = r * (r - 1) / 2;              /* sum of lengths of preceding rows */
    idx_t k_target   = n - prev_sum;                /* 1‑based column within the row   */
    idx_t sz = r + 1;                                /* table dimensions 0..r           */

    /* Allocate storage for the triangle T[n][k] */
#ifdef __cplusplus
    std::vector<seq_t> T(sz * sz);
#else
    seq_t *T = malloc(sz * sz * sizeof(seq_t));
    if (!T) return 0;
#endif

    /* Allocate Catalan numbers needed for the current row   */
#ifdef __cplusplus
    std::vector<seq_t> cat(r);                       /* indices 0..r-1, cat[0] unused   */
#else
    seq_t *cat = malloc(r * sizeof(seq_t));
    if (!cat) { free(T); return 0; }
#endif

    /* Fill cat[h] = catalan(h) for 1 ≤ h < r */
    for (idx_t h = 1; h < r; ++h) {
        cat[h] = catalan(h);
    }

    /* Base row: T[0][k] = 1 for all k ≥ 1 */
    for (idx_t k = 1; k <= r; ++k) {
        T[k] = (seq_t)1;   /* T[0][k] */
    }

    /* Dynamic programming for T[m][k] */
    for (idx_t m = 1; m <= r; ++m) {
        for (idx_t k = 1; k <= r; ++k) {
            seq_t sum = (seq_t)2 * T[(m - 1) * sz + k];
            for (idx_t h = 1; h <= k - 1; ++h) {
                if (m > h) {
                    seq_t prod = T[(m - h - 1) * sz + k];
                    prod *= cat[h];
                    sum += prod;
                }
            }
            T[m * sz + k] = sum;
        }
    }

    seq_t result = T[r * sz + k_target];

#ifndef __cplusplus
    free(cat);
    free(T);
#endif

    return result;
}
