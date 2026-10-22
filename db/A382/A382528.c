/*
 * A382528  Triangle T(n,k) read by rows, where row n is a permutation of the
 *          numbers 1 through n, such that if a deck of n cards is prepared in
 *          this order, and under-down-down dealing is used, then the
 *          resulting cards will be dealt in increasing order.
 * https://oeis.org/A382528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle read row by row. T(n,k)
 *            = T(n-2,k-3) + 2 (n>3, k>3) Base: T(1,1)=1; T(n,2)=1; T(n,3)=2.
 *            Here a(n) = T(r,c) where r is the row index such that r(r+1)/2
 *            >= n and c = n - r(r-1)/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of the triangle in a one‑dimensional array. */
static seq_t *triangle = NULL;        /* 0‑based linear storage        */
static idx_t  current_rows = 0;       /* largest fully constructed row */

/* Find the row r such that r*(r+1)/2 >= n. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until it is large enough. */
    while (high * (high + 1) / 2 < n) high <<= 1;

    /* Binary search for the smallest r. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Grow the cached triangle to include all rows up to r (inclusive). */
static void extend_triangle(idx_t r)
{
    /* Determine new size of the flattened array. */
    size_t new_size = (size_t)r * (r + 1) / 2;
    seq_t *new_data = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));
    if (!new_data) {
        /* Out of memory – abort the program. */
        exit(EXIT_FAILURE);
    }
    triangle = new_data;

    /* Build any rows that were not previously computed. */
    for (idx_t rr = current_rows + 1; rr <= r; ++rr) {
        size_t offset = (size_t)rr * (rr - 1) / 2;          /* start index of row rr */
        if (rr == 1) {
            triangle[offset] = (seq_t)1;
        } else if (rr == 2) {
            triangle[offset]     = (seq_t)2;   /* T(2,1) */
            triangle[offset + 1] = (seq_t)1;   /* T(2,2) */
        } else if (rr == 3) {
            triangle[offset]     = (seq_t)3;   /* T(3,1) */
            triangle[offset + 1] = (seq_t)1;   /* T(3,2) */
            triangle[offset + 2] = (seq_t)2;   /* T(3,3) */
        } else {
            size_t prev_offset = (size_t)(rr - 2) * (rr - 3) / 2; /* start of row rr-2 */

            /* column 1: T(rr,1) = T(rr-2,rr-2) + 2 */
            triangle[offset] = triangle[prev_offset + (rr - 3)] + (seq_t)2;

            /* column 2 is always 1 */
            triangle[offset + 1] = (seq_t)1;

            /* column 3 is always 2 */
            triangle[offset + 2] = (seq_t)2;

            /* columns 4..rr use the recurrence */
            for (idx_t k = 4; k <= rr; ++k) {
                size_t src = prev_offset + (k - 4);      /* T(rr-2, k-3) */
                size_t dst = offset + (k - 1);
                triangle[dst] = triangle[src] + (seq_t)2;
            }
        }
    }
    current_rows = r;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n<1 – return 0 */

    idx_t row = find_row(n);                /* row containing the n‑th entry */
    if (row > current_rows)                /* build missing rows if needed */
        extend_triangle(row);

    size_t offset = (size_t)row * (row - 1) / 2; /* start index of the row */
    idx_t  col    = n - offset;                /* 1‑based column within the row */
    return triangle[offset + (col - 1)];
}
