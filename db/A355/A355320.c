/*
 * A355320  Irregular triangle T(n, k), n >= 0, -2*n <= k <= 2*n, read by
 *          rows; T(0, 0) = 1; for n > 0, T(n, k) is the sum of all terms in
 *          previous rows at one knight's move away.
 * https://oeis.org/A355320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice, look
 * Formula:   a(n) = T(r, k) where r = minimal integer with 2r^2+3r+1 > n k =
 *            -2r + (n - (2(r-1)^2+3(r-1)+1)) The triangle entries T(n,k)
 *            satisfy T(0,0)=1,
 *            T(n,k)=T(n-1,k-2)+T(n-1,k+2)+T(n-2,k-1)+T(n-2,k+1) (terms
 *            outside the domain are treated as zero). The function computes
 *            a(n) by building rows of the trian...
 * Range:     u8 n <= 135, u16 n <= 404, u32 n <= 1368, u64 n <= 4647,
 *            u128 n <= 9775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <cstddef>
inline seq_t *alloc_seq(size_t n) { return new seq_t[n]; }
inline void free_seq(seq_t *p) { delete[] p; }
#else
#include <stddef.h>
static inline seq_t *alloc_seq(size_t n) { return (seq_t*)calloc(n, sizeof(seq_t)); }
static inline void free_seq(seq_t *p) { free(p); }
#endif
#include <stdlib.h>
#include <stdint.h>

/* Compute the nth term of A355320. */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------
     * Determine the row r that contains the index n.
     * The cumulative number of elements up to and including row r is
     *   C(r) = 2r^2 + 3r + 1
     * Find the smallest r with C(r) > n.
     * -------------------------------------------------------------- */
    idx_t r = 0;
    while ((2 * r * r + 3 * r + 1) <= n) ++r;

    /* --------------------------------------------------------------
     * Allocate arrays for the last two rows.  The maximum row width
     * needed is 4*r + 1.
     * -------------------------------------------------------------- */
    size_t max_width = (size_t)(4 * r + 1);
    seq_t *row_nm2 = alloc_seq(max_width);   /* row n-2  (empty for n=0,1)   */
    seq_t *row_nm1 = alloc_seq(max_width);   /* row n-1  (row 0 initially) */

    /* Row 0 */
    row_nm1[0] = (seq_t)1;

    /* If the target is row 0, the answer is already ready. */
    if (r == 0) {
        seq_t result = row_nm1[0];
        free_seq(row_nm2);
        free_seq(row_nm1);
        return result;
    }

    /* --------------------------------------------------------------
     * Generate rows from 1 up to r, keeping only the last two rows.
     * The recurrence simplifies when expressed with index i (the
     * position within the current row):
     *   T(n,i) = T(n-1,i-4) + T(n-1,i) + T(n-2,i-5) + T(n-2,i-3)
     * indices outside the valid range are treated as zero.
     * -------------------------------------------------------------- */
    for (idx_t current = 1; current <= r; ++current) {
        seq_t *row_n = alloc_seq(max_width);   /* current row */
        size_t sz = (size_t)(4 * current + 1);

        size_t sz_nm1 = (size_t)(4 * (current - 1) + 1);
        size_t sz_nm2 = 0;
        if (current >= 2) sz_nm2 = (size_t)(4 * (current - 2) + 1);

        for (size_t i = 0; i < sz; ++i) {
            seq_t sum = 0;

            /* T(n-1,i-4) */
            if (i >= 4) {
                size_t off = i - 4;
                if (off < sz_nm1) sum += row_nm1[off];
            }
            /* T(n-1,i) */
            if (i < sz_nm1) sum += row_nm1[i];

            /* T(n-2,i-5) and T(n-2,i-3) only for current >= 2 */
            if (current >= 2) {
                if (i >= 5) {
                    size_t off = i - 5;
                    if (off < sz_nm2) sum += row_nm2[off];
                }
                if (i >= 3) {
                    size_t off = i - 3;
                    if (off < sz_nm2) sum += row_nm2[off];
                }
            }

            row_n[i] = sum;
        }

        /* Shift rows: previous rows become n-2 and n-1 for next iteration. */
        free_seq(row_nm2);
        row_nm2 = row_nm1;
        row_nm1 = row_n;
    }

    /* --------------------------------------------------------------
     * Extract the required element from the last computed row.
     * -------------------------------------------------------------- */
    seq_t result;
    if (r == 0) {
        result = row_nm1[0];
    } else {
        idx_t prev_cum = (idx_t)(2 * (r - 1) * (r - 1) + 3 * (r - 1) + 1);
        idx_t offset = n - prev_cum;
        result = row_nm1[(size_t)offset];
    }

    free_seq(row_nm2);
    free_seq(row_nm1);
    return result;
}
