/*
 * A379460  Array read by downward antidiagonals: A(n,k) = A(n,k-1) +
 *          (k+1)*(A(n-1,k) + A(n-1,k+1)) with A(n,0) = A(n-1,0) + A(n-1,1),
 *          A(0,k) = 1, n >= 0, k >= 0.
 * https://oeis.org/A379460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangular array A(n,k) defined
 *            by A(0,k) = 1 for all k ≥ 0 A(n,0) = A(n-1,0) + A(n-1,1) for n ≥
 *            1 A(n,k) = A(n,k-1) + (k+1)*(A(n-1,k)+A(n-1,k+1)) for n ≥ 1 , k
 *            ≥ 1 . The sequence a(n) is the concatenation of the values along
 *            the downward antidiagonals (row+col = c...
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, required_d_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required includes */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/required_d_from_idx.h"

/* cache for the flattened sequence */
static seq_t *a_vals   = NULL;   /* array of a(n) values */
static size_t a_cap    = 0;      /* number of elements stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* return cached value if already present */
    if (a_vals && n < a_cap)
        return a_vals[n];

    /* determine maximal needed diagonal */
    idx_t dmax = required_d_from_idx(n);
    size_t width      = (size_t)dmax + 1;                 /* columns per row */
    size_t total_terms = (size_t)(dmax + 1) * (dmax + 2) / 2;

    /* allocate new array for a(n) values */
    seq_t *new_a_vals = alloc_seq_array(total_terms);
    if (!new_a_vals)
        return 0;   /* allocation failure */

    /* allocate triangle buffer */
    size_t tri_size = (size_t)(dmax + 1) * width;
    seq_t *triangle = alloc_seq_array(tri_size);
    if (!triangle) {
        free_seq_array(new_a_vals);
        return 0;
    }

    /* base row n = 0 : A(0,k) = 1 for all k */
    for (size_t col = 0; col < width; ++col)
        triangle[col] = 1;

    /* fill rows 1 … dmax */
    for (idx_t row = 1; row <= dmax; ++row) {
        size_t kmax = dmax - row;                    /* max column for this row */
        size_t cur_offset  = (size_t)row * width;
        size_t prev_offset = (size_t)(row - 1) * width;

        /* k = 0 : A(row,0) */
        triangle[cur_offset] = triangle[prev_offset] + triangle[prev_offset + 1];

        /* k = 1 … kmax : A(row,k) */
        for (size_t k = 1; k <= kmax; ++k) {
            seq_t left = triangle[cur_offset + k - 1];
            seq_t up   = triangle[prev_offset + k];
            seq_t up1  = triangle[prev_offset + k + 1];
            triangle[cur_offset + k] = left + (seq_t)(k + 1) * (up + up1);
        }
    }

    /* flatten the triangle along downward antidiagonals */
    size_t idx = 0;
    for (idx_t d = 0; d <= dmax; ++d) {
        for (idx_t row = 0; row <= d; ++row) {
            size_t col = d - row;
            new_a_vals[idx++] = triangle[row * width + col];
        }
    }

    /* replace old cache */
    if (a_vals)
        free_seq_array(a_vals);
    a_vals = new_a_vals;
    a_cap  = total_terms;

    return a_vals[n];
}
