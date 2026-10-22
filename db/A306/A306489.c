/*
 * A306489  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals, where
 *          column k is the expansion of 1/(1 - Sum_{d|k} x^d).
 * https://oeis.org/A306489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A306489:  a(n) = number of compositions of r into divisors of k,
   where (r,k) is the pair corresponding to index n on the antidiagonal
   read in the square array A(n,k).  The recurrence is :

        A(0, k) = 1
        A(r, k) = Σ_{d|k, d ≤ r} A(r − d, k)

   The first values (n, a(n)) are:
        (0,1) (1,1) (2,1) (3,1) (4,1) (5,1) (6,1) (7,1) (8,2) (9,1) ...
*/

#include <stdlib.h>
#include <string.h>

/*  Static storage for the pre‑computed table.  */
static seq_t *data    = NULL;  /* 1‑D continuous buffer   */
static idx_t cur_s    = 0;    /* number of diagonals currently filled (s‑1)  */
static size_t stride  = 0;    /* stride = cur_s + 1 for row indexing     */

/*  Ensure that the table is grown to support diagonals up to 's'. */
static void ensure_s(idx_t s)
{
    if (s <= cur_s)
        return;

    idx_t old_s = cur_s;
    idx_t new_s = s;
    size_t new_stride = (size_t)new_s + 1;

    /* Allocate new buffer and clear it.  */
    seq_t *new_data = (seq_t *)calloc((size_t)new_s * new_stride, sizeof(seq_t));
    if (!new_data)
        abort();   /* memory exhaustion */

    /* Copy existing data into the new layout.  */
    if (old_s > 0) {
        for (idx_t r = 0; r < old_s; ++r) {
            memcpy(&new_data[r * new_stride],
                   &data[r * (old_s + 1)],
                   (size_t)(old_s + 1) * sizeof(seq_t));
        }
    }

    /* Compute values for all columns k = 1 .. new_s and rows
       r = 0 .. new_s-1 using the recurrence.  The recurrence
       only uses values from the same column, so a simple
       ascending loop over r is sufficient.  */
    for (idx_t k = 1; k <= new_s; ++k) {
        new_data[0 * new_stride + k] = 1;   /* A(0,k)=1 */
        for (idx_t r = 1; r < new_s; ++r) {
            seq_t sum = 0;
            for (idx_t d = 1; d <= k; ++d) {
                if ((k % d) == 0 && d <= r) {
                    sum += new_data[(r - d) * new_stride + k];
                }
            }
            new_data[r * new_stride + k] = sum;
        }
    }

    free(data);
    data   = new_data;
    cur_s  = new_s;
    stride = new_stride;
}

/*  Main sequence entry point.  */
seq_t a(idx_t n)
{
    /* Find the antidiagonal s that contains the index n.
       The diagonal of sum s has indices from
       start = s*(s-1)/2  up to  end   = s*(s+1)/2 - 1.       */
    idx_t s   = 1;
    idx_t cum = 1;          /* cumulative size so far   */
    while (cum <= n) {
        ++s;
        cum += s;
    }

    /* Compute start index of block s and offset within the block.  */
    idx_t start  = (s - 1) * s / 2;
    idx_t offset = n - start;        /* 0 ≤ offset < s */
    idx_t r = offset;                /* row index   */
    idx_t c = s - offset;            /* column index (≥1)   */

    /* Ensure our table reaches at least this diagonal.  */
    ensure_s(s);

    /* Retrieve the stored value.  */
    return data[r * stride + c];
}
