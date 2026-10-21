/*
 * A283741  Array read by descending antidiagonals: row k lists the numbers m
 *          such that 1/2^(k+1) < 1 - f(m) < 1/2^k, where f(m) is the
 *          fractional part of m*(golden ratio).
 * https://oeis.org/A283741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   row_of_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the element of the infinite table read by descending antidiagonals
 * where table is defined by:
 *   row k contains all positive integers m such that
 *     1/2^(k+1) < 1 - frac(m*φ) < 1/2^k
 *   φ  = (1 + sqrt(5))/2 (the golden ratio)
 *
 *   The table is read by antidiagonals:
 *     (0,0), (0,1),(1,0), (0,2),(1,1),(2,0), …
 *
 * Implementation details:
 *   - To determine the row of a given integer m we avoid floating‑point arithmetic
 *     by using a 64‑bit rational approximation of φ:
 *        φ ≈ A/B  with A=12586269025, B=7778742049 (F_50 / F_49)
 *   - The fractional part of m*φ is (m*A mod B) / B.
 *   - The value t = 1 - frac(m*φ) = (B - (m*A mod B)) / B.
 *   - The row k is the unique integer satisfying
 *         (t_num << k) < B  and  (t_num << (k+1)) > B
 *     where t_num = B - (m*A mod B).
 *
 *   - We generate all rows up to the maximum row index needed to produce the
 *     first n terms.  For each such row k we preallocate an array of the
 *     exact number of elements that will be required for the first n terms.
 *   - After all rows are filled we enumerate the table by antidiagonals
 *     until the requested index n is reached.
 *
 *   The algorithm runs in O(n) time and uses O(n) memory, easily fast enough
 *   for n up to 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_of_m.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1, undefined for 0, but return 0 safely */
        return (seq_t)0;

    /* Find the minimal diagonal index d such that (d+1)(d+2)/2 >= n   */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2) / 2) < n )
        ++d;
    idx_t dmax = d;            /* maximum row index needed   */
    idx_t kmax = dmax;

    /* Allocate arrays for each row: need[k] elements per row                */
    idx_t *need = (idx_t*)calloc((size_t)(kmax + 1), sizeof(idx_t));
    seq_t **rows   = (seq_t**)calloc((size_t)(kmax + 1), sizeof(seq_t*));
    idx_t *size    = (idx_t*)calloc((size_t)(kmax + 1), sizeof(idx_t));

    /* Determine required size for each row */
    for (idx_t k = 0; k <= kmax; ++k)
        need[k] = dmax - k + 1;                 /* j ranges 0..d-k  */

    /* Allocate per-row storage with exact capacity                      */
    for (idx_t k = 0; k <= kmax; ++k) {
        if (need[k] > 0)
            rows[k] = (seq_t*)calloc((size_t)need[k], sizeof(seq_t));
    }

    /* Fill rows until every required element has been produced          */
    idx_t remaining = kmax + 1;   /* rows that still need elements          */
    uint64_t m = 1;
    while (remaining > 0) {
        int k = row_of_m(m);
        if (k >= 0 && (idx_t)k <= kmax) {
            if (size[k] < need[k]) {
                rows[k][size[k]] = (seq_t)m;
                ++size[k];
                if (size[k] == need[k])
                    --remaining;
            }
        }
        ++m;
    }

    /* Enumerate the table by antidiagonals and return the n-th element   */
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t diag = 0; ; ++diag) {
        for (idx_t k = 0; k <= diag; ++k) {
            if (k > kmax)
                continue;
            idx_t j = diag - k;
            if (j < need[k]) {
                ++count;
                if (count == n) {
                    result = rows[k][j];
                    goto cleanup;
                }
            }
        }
    }

cleanup:
    for (idx_t k = 0; k <= kmax; ++k) {
        free(rows[k]);
    }
    free(rows);
    free(size);
    free(need);

    return result;
}
