/*
 * A290319  Triangle read by rows: T(n, k) is the Sheffer triangle ((1 -
 *          4*x)^(-1/4), (-1/4)*log(1 - 4*x)). A generalized Stirling1
 *          triangle.
 * https://oeis.org/A290319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 119, u128 n <= 324,
 *            big any n
 * Helpers:   mul_by_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A290319: Triangle read by rows.
   T(0,0) = 1
   Recurrence: T(n,k)=T(n-1,k-1)+(4*n-3)*T(n-1,k)   (n≥1, 0≤k≤n,
   T(n,-1)=0, T(n,k)=0 for k>n).
   a(n) = T(r,c) where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and
   c = n - r*(r+1)/2.
   The function works for all indices n≥0 and is memoised for
   arbitrary order of calls.  It uses O(n) time and memory.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_by_int.h"

/* dynamic storage for the triangle rows */
static seq_t **triangle_rows   = NULL;   /* array of pointers to rows   */
static idx_t   row_capacity    = 0;      /* allocated pointer array size */
static idx_t   max_row_computed = 0;     /* highest row currently built */

/* initialise the triangle with row 0 */
static void init_triangle(void)
{
    row_capacity = 1;
    triangle_rows = (seq_t**)malloc(row_capacity * sizeof(seq_t*));
    triangle_rows[0] = (seq_t*)malloc(sizeof(seq_t));
    triangle_rows[0][0] = (seq_t)1;          /* T(0,0)=1 */
    max_row_computed = 0;
}

/* make sure the pointer array can hold indices up to 'needed' */
static void ensure_row_capacity(idx_t needed)
{
    if (needed < row_capacity)
        return;
    idx_t new_cap = row_capacity ? row_capacity * 2 : 1;
    while (new_cap <= needed)
        new_cap *= 2;
    seq_t **tmp = (seq_t**)realloc(triangle_rows, new_cap * sizeof(seq_t*));
    if (!tmp)
        abort();   /* out of memory */
    /* initialise new slots to NULL */
    for (idx_t i = row_capacity; i < new_cap; ++i)
        tmp[i] = NULL;
    triangle_rows = tmp;
    row_capacity = new_cap;
}

/* find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static inline idx_t row_for_index(idx_t n)
{
    idx_t r = 0;
    /* simple loop is fast for n≤10000; for larger n use binary search if needed */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* build all rows up to and including row r */
static void build_rows_upto(idx_t r)
{
    if (r <= max_row_computed)
        return;

    ensure_row_capacity(r);

    for (idx_t i = max_row_computed + 1; i <= r; ++i) {
        /* allocate storage for row i (length i+1) */
        triangle_rows[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        seq_t *row_prev = triangle_rows[i - 1];
        idx_t coeff = 4 * i - 3;      /* (4*i-3) is safe for i≥1 */

        for (idx_t k = 0; k <= i; ++k) {
            seq_t left  = (k > 0) ? row_prev[k - 1] : (seq_t)0;
            seq_t right = (k < i) ? row_prev[k]     : (seq_t)0;
            seq_t prod  = mul_by_int(right, coeff);
            triangle_rows[i][k] = left + prod;
        }
    }
    max_row_computed = r;
}

/* public function: a(n) */
seq_t a(idx_t n)
{
    if (triangle_rows == NULL)
        init_triangle();

    idx_t r = row_for_index(n);
    if (r > max_row_computed)
        build_rows_upto(r);

    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;
    return triangle_rows[r][k];
}
