/*
 * A364039  Triangle read by rows: T(n,k) is the number of integer
 *          compositions of n with first part k and differences between
 *          neighboring parts in {-1,1}.
 * https://oeis.org/A364039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained from the triangle T(r,k) where T(r,r) = 1,
 *            T(r,0) = 0 for r>0, T(r,k) = T(r-k,k+1) + T(r-k,k-1) for 0<k<r.
 *            For a given flattened index n the corresponding row r and column
 *            k are r = floor( (sqrt(8*n+1) - 1)/2 ) k = n - r*(r+1)/2 . The
 *            function returns T(r,k)
 * Range:     u8 n <= 497, u16 n <= 1654, u32 n <= 5996, u64 n <= 8571,
 *            u128 n <= 8571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Forward declaration of the public function */
seq_t a(idx_t n);

/* Private static data */
static seq_t *tri = NULL;          /* flattened triangle table */
static idx_t  max_row = 0;         /* highest row computed so far */

/* Ensure that rows 0..row are computed and stored in tri */
static void ensure_rows(idx_t row)
{
    if (row <= max_row) return;

    /* compute new capacity of the flattened array
       (row+1)*(row+2)/2  */
    size_t new_cap = (size_t)(row + 1) * (row + 2) / 2;
    size_t old_cap = (size_t)(max_row + 1) * (max_row + 2) / 2;

    if (tri == NULL) {
        tri = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        tri = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
    }

    /* Build rows from max_row+1 up to row */
    for (idx_t r = max_row + 1; r <= row; ++r)
    {
        size_t start = (size_t)r * (r + 1) / 2;   /* index of T(r,0) */

        /* base cases */
        tri[start] = (r == 0) ? (seq_t)1 : (seq_t)0;  /* T(r,0) */
        if (r > 0) tri[start + r] = (seq_t)1;          /* T(r,r) = 1 */

        /* interior elements */
        for (idx_t k = 1; k < r; ++k)
        {
            idx_t rmk = r - k;
            size_t start_other = (size_t)rmk * (rmk + 1) / 2;

            seq_t val = (seq_t)0;

            if (k + 1 <= rmk)
                val += tri[start_other + (k + 1)];
            if (k - 1 <= rmk)
                val += tri[start_other + (k - 1)];

            tri[start + k] = val;   /* T(r,k) */
        }
    }

    max_row = row;
}

/* Public function */
seq_t a(idx_t n)
{
    /* determine row r and column k from flattened index n */
    if (n == 0) return (seq_t)1;

    double d   = sqrt((double)8 * (double)n + 1.0);
    idx_t  r    = (idx_t)((d - 1.0) / 2.0);

    /* correct r if needed (safe against round–off) */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t  k = n - r * (r + 1) / 2;

    /* ensure the required rows are computed */
    ensure_rows(r);

    size_t index = (size_t)r * (r + 1) / 2 + k;
    return tri[index];
}
