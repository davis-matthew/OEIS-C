/*
 * A276696  Triangle read by rows, T(n,k) = T(n-1, k-1) + T(n-2, k) if k is
 *          odd, T(n-1, k-1) + T(n-1, k) if k is even, for k<=0<=n and n>=2
 *          with T(0,0)=T(1,0)=T(1,1)=0 and T(n,k)=0 when k>n, k<0, or n<0.
 * https://oeis.org/A276696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   Sequence A276696 – Triangle read by rows

   T(n,k) = { T(n-1,k-1) + T(n-2,k)   if k is odd,
            { T(n-1,k-1) + T(n-1,k)   if k is even }

   with T(0,0) = 1, T(1,0) = T(1,1) = 1, and T(n,k)=0 for k>n, k<0 or n<0.
   The sequence a(n) is obtained by traversing the triangle row‑wise:
   a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ...

   The code below builds the triangle on demand and returns a(n) for any
   n≥0.  It uses only integer arithmetic and works with the generic
   integer types `seq_t` and `idx_t` supplied by the build system.

*/

#include <stdlib.h>   /* malloc, realloc, abort, size_t */
#include <stdint.h>   /* for uintptr_t cast */

static seq_t **triangle = NULL;    /* array of pointers to each row */
static size_t triangle_cap = 0;    /* current capacity of the triangle array */
static idx_t triangle_rows = 0;    /* number of rows that have been computed */

/* allocate space for a new row and compute all its entries */
static void compute_row(idx_t r)
{
    /* grow the array of row pointers if necessary */
    if (triangle_cap <= (size_t)r) {
        size_t new_cap = triangle_cap ? triangle_cap << 1 : 1;
        while (new_cap <= (size_t)r) new_cap <<= 1;
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
        triangle_cap = new_cap;
    }

    /* allocate storage for the row itself */
    triangle[r] = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!triangle[r]) abort();

    if (r == 0) {
        triangle[0][0] = 1;
        return;
    }
    if (r == 1) {
        triangle[1][0] = 1;
        triangle[1][1] = 1;
        return;
    }

    /* general case: r >= 2 */
    for (idx_t k = 0; k <= r; ++k) {
        seq_t left = 0, right = 0;

        if (k == 0) {
            /* left component is T(r-1,-1) which is 0,
               right component depends on k (even) */
            if (r >= 1) right = triangle[r - 1][0];
            triangle[r][0] = right;
            continue;
        }

        /* T(r-1,k-1) always exists for k>=1 */
        left = triangle[r - 1][k - 1];

        if ((k & 1) == 0) {            /* k even */
            if (k <= r - 1) right = triangle[r - 1][k];
        } else {                       /* k odd */
            if (k <= r - 2) right = triangle[r - 2][k];
        }

        triangle[r][k] = left + right;
    }
}

/* ensure that all rows up to and including r have been computed */
static void ensure_triangle_up_to(idx_t r)
{
    while (triangle_rows <= r) {
        compute_row(triangle_rows);
        ++triangle_rows;
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    /* find the row r such that the flattened index n lies in row r */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum <= n) {
        sum += r + 1;
        ++r;
    }
    --r;
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    /* compute the triangle up to row r if needed */
    ensure_triangle_up_to(r);

    return triangle[r][k];
}
