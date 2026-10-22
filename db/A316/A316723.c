/*
 * A316723  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j=1..k} (1+x^j)/(1-x^j).
 * https://oeis.org/A316723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 809, u32 n <= 5757, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   d_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A316723 – coefficient of x^i in the j‑th column of
    ∏_{k=1..j} (1+x^k)/(1‑x^k), where (i,j) are the
    antidiagonal coordinates of n.
    (This implementation builds the table incrementally.) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/d_of_idx.h"

/* Forward declaration (provided by the build system) */
typedef uint64_t idx_t;      /* index type */

/* Global cache for the coefficient table.
   coeff_table[row * grid_side + col] holds the coefficient
   of x^col in the row‑th column. */
static seq_t *coeff_table = NULL;
static size_t grid_side = 0;    /* current width of the table (number of columns) */

/* Ensure the table has size at least new_size (maximum index inclusive).
   A new table of side (new_size + 1) × (new_size + 1) is allocated
   and previous data is discarded.  The table is rebuilt from scratch. */
static void ensure_coeff_table(size_t new_size)
{
    if (new_size + 1 <= grid_side)
        return;               /* already big enough */

    size_t side = new_size + 1;
    seq_t *new_table = (seq_t *)malloc(side * side * sizeof(seq_t));
    if (!new_table) abort();   /* allocation failure */

    /* initialise column 0 : product of zero factors = 1 */
    for (size_t col = 0; col < side; ++col)
        new_table[col] = (seq_t)(col == 0 ? 1 : 0);

    /* build subsequent columns */
    for (size_t k = 1; k < side; ++k) {
        seq_t *prev = new_table + (k - 1) * side;   /* previous column */
        seq_t *curr = new_table + k * side;         /* current column   */
        for (size_t i = 0; i < side; ++i) {
            seq_t val = prev[i];                    /* contribution from m = 0 */
            for (size_t m = 1; m * k <= i; ++m)
                val += (seq_t)2 * prev[i - m * k];
            curr[i] = val;
        }
    }

    if (coeff_table) free(coeff_table);
    coeff_table = new_table;
    grid_side = side;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Convert index to antidiagonal coordinates (i,j). */
    idx_t d = d_of_idx(n);
    idx_t tri = d * (d + 1) / 2;   /* triangular number T_d */
    idx_t i = n - tri;             /* row index   (exponent of x)   */
    idx_t j = d - i;               /* column index (product length) */

    /* Ensure we have computed the table up to these indices. */
    size_t needed = (i > j) ? i : j;   /* maximum index needed */
    ensure_coeff_table(needed);

    /* Return the requested coefficient: coefficient of x^i in column j. */
    return coeff_table[j * grid_side + i];
}
