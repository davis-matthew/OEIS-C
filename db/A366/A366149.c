/*
 * A366149  Triangle read by rows. T(n, k) = A000566(n - k + 1) * T(n, k - 1)
 *          + T(n - 1, k) for 0 < k < n. T(n, 0) = 1 and T(n, n) = T(n, n - 1)
 *          if n > 0.
 * https://oeis.org/A366149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows. Recurrence:
   T(0,0)=1,   T(n,0)=1 (n>0),
   T(n,n)=T(n,n-1) (n>0),
   T(n,k)=A000566(n-k+1)*T(n,k-1)+T(n-1,k)  for 0<k<n.
   a(n)=T(row_of(n), n-row_start(row_of(n))), with row_start(r)=r*(r+1)/2.
   A000566(m)=m*(5*m-3)/2. */
#include <stdlib.h>

/* Static storage for the triangle: triangle[i] points to array of i+1 T(i,k). */
static seq_t **triangle = NULL;
static size_t triangle_rows = 0;  /* number of rows currently computed */
static size_t triangle_cap  = 0;  /* capacity of the triangle array */

/* Ensure the pointer array can hold rows up to r (inclusive). */
static void ensure_triangle_capacity(size_t r)
{
    if (r >= triangle_cap) {
        size_t new_cap = triangle_cap ? triangle_cap * 2 : 8;
        while (new_cap <= r) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t *));
        if (!tmp) abort();
        triangle = tmp;
        triangle_cap = new_cap;
    }
}

/* Return the row index containing position n in the row‑by‑row reading. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    return r;
}

/* Compute a(n).  Lazily generates rows up to the needed row and stores them. */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    ensure_triangle_capacity((size_t)r);

    /* Build any missing rows up to r. */
    for (size_t i = triangle_rows; i <= (size_t)r; ++i) {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort();

        row[0] = (seq_t)1;  /* T(i,0) = 1 */

        if (i > 0) {
            seq_t *prev = triangle[i - 1];
            for (idx_t k = 1; k < i; ++k) {
                idx_t m = i - k + 1;  /* argument for the heptagonal number */
                seq_t w = (seq_t)m * ((seq_t)(5 * m - 3)) / 2;  /* A000566(m) */
                row[k] = w * row[k - 1] + prev[k];
            }
            row[i] = row[i - 1];  /* T(i,i) = T(i,i-1) */
        }

        triangle[i] = row;
    }
    triangle_rows = r + 1;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return triangle[r][k];
}
