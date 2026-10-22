/*
 * A306309  The "zeroless Pascal triangle" read by rows.
 * https://oeis.org/A306309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   removeZeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A306309: zeroless Pascal triangle read by rows
   a(0)=1, a(1)=1, …
   T(n,0)=T(n,n)=1
   T(n,k)=removeZeros(T(n-1,k-1)+T(n-1,k))   (1 ≤ k < n)
   a(n) is the element T(r,p) where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
   and p = n – r*(r+1)/2.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/removeZeros.h"

/* seq_t and idx_t are defined elsewhere by the build system. */

/* binary search for row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

/* cached rows of the zeroless Pascal triangle */
static seq_t **triangle = NULL;     /* array of pointers, each points to a row */
static size_t triangle_len = 0;     /* number of rows currently allocated */
static idx_t  max_computed_row = 0; /* highest row that has been computed */

/* ensure that rows 0..r are computed and stored in triangle */
static void ensure_rows(idx_t r)
{
    /* initialise the storage if this is the first call */
    if (triangle_len == 0) {
        triangle_len = 1;
        triangle = (seq_t **) malloc(sizeof(seq_t *));
        triangle[0] = (seq_t *) malloc(sizeof(seq_t));
        triangle[0][0] = 1;
        max_computed_row = 0;
    }

    if (r <= max_computed_row) return;

    /* extend the array of row pointers if necessary */
    if (triangle_len <= (size_t)r) {
        size_t new_len = r + 1;
        triangle = (seq_t **) realloc(triangle, new_len * sizeof(seq_t *));
        for (size_t i = triangle_len; i < new_len; ++i) triangle[i] = NULL;
        triangle_len = new_len;
    }

    /* compute all missing rows one by one */
    for (idx_t row = max_computed_row + 1; row <= r; ++row) {
        triangle[row] = (seq_t *) malloc((row + 1) * sizeof(seq_t));

        /* left and right edges are 1 */
        triangle[row][0] = 1;
        triangle[row][row] = 1;

        /* interior entries */
        for (idx_t k = 1; k < row; ++k) {
            seq_t sum = triangle[row - 1][k - 1] + triangle[row - 1][k];
            triangle[row][k] = removeZeros(sum);
        }
    }

    max_computed_row = r;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t pos = n - (idx_t)((seq_t)r * ((seq_t)r + 1) / 2);

    ensure_rows(r);

    return triangle[r][pos];
}
