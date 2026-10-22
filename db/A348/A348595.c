/*
 * A348595  Triangle read by rows: Number of walks from (0,0) to (3n,3k) on
 *          the square lattice with up and right steps where squares
 *          (x,y)=(1,1) mod 3 or (x,y)=(2,2) mod 3 are not entered.
 * https://oeis.org/A348595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   The triangle T(u,v)= (1 – u v)/(1 – u – v – 3 u v) yields
 *            coefficients a[i][j] defined by a[0][0] = 1 a[i][0] = a[0][j] =
 *            1 (i,j ≥ 0) a[i][j] = a[i-1][j] + a[i][j-1] + 3*a[i-1][j-1]
 *            (i>0,j>0) with the special case a[1][1] := a[1][1] – 1 to
 *            account for the “–u v” term. The sequence a(n) is the tri...
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   linear_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/linear_to_rc.h"

/* static cache for the current triangle table */
static seq_t *T = NULL;            /* flattened array of size dim*dim */
static idx_t   dim = 0;            /* table dimension (last computed row + 1) */

/* Build the table a[i][j] for 0 ≤ i,j ≤ R (inclusive). */
static void compute_table(idx_t R)
{
    idx_t   newDim = R + 1;
    size_t  sz     = (size_t)newDim * (size_t)newDim;
    seq_t  *newT   = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!newT) exit(1);              /* out of memory */

    for (idx_t i = 0; i < newDim; ++i) {
        for (idx_t j = 0; j < newDim; ++j) {
            size_t idx = (size_t)i * (size_t)newDim + (size_t)j;
            if (i == 0 && j == 0) {
                newT[idx] = 1;
            } else if (i == 0 || j == 0) {
                newT[idx] = 1;
            } else {
                seq_t a = newT[(size_t)(i-1) * (size_t)newDim + (size_t)j];
                seq_t b = newT[(size_t)i * (size_t)newDim + (size_t)(j-1)];
                seq_t c = newT[(size_t)(i-1) * (size_t)newDim + (size_t)(j-1)];
                seq_t val = a + b + 3 * c;
                if (i == 1 && j == 1) val -= 1;   /* handles the –u v term */
                newT[idx] = val;
            }
        }
    }

    /* free old table */
    if (T) free(T);
    T   = newT;
    dim = newDim;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t r, p;
    linear_to_rc(n, &r, &p);

    /* Ensure table is built up to row r */
    if (dim <= r) compute_table(r);

    /* Index in flattened array */
    size_t idx = (size_t)r * (size_t)dim + (size_t)p;
    return T[idx];
}
