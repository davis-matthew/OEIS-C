/*
 * A381622  Triangle T(n,k) read by rows, where row n is a permutation of the
 *          numbers 1 through n, such that if a deck of n cards is prepared in
 *          this order, and down-under-under dealing is used, then the
 *          resulting cards will be dealt in increasing order.
 * https://oeis.org/A381622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for OEIS A381622 (triangle T(n,k)):
 *
 *   T(n,1) = 1
 *   T(2,2) = 2
 *   For n > 2:
 *     T(n,2) = T(n-1,n-2) + 1
 *     T(n,3) = T(n-1,n-1) + 1
 *     T(n,k) = T(n-1,k-3) + 1   for k > 3
 *
 *  The sequence a(n) is the row‑major flattening of this triangle.
 *  Equivalent:  a(n) = T(r,c) where r is the unique integer with
 *                 r*(r-1)/2 < n ≤ r*(r+1)/2   (row number)
 *                 c = n − r*(r-1)/2           (column number)
 *
 *  The function below computes a(n) by first locating (r,c) and then
 *  evaluating T(r,c) by a simple recursive descent following the
 *  recurrence.  This keeps memory usage O(1) and is fast for n up to 10⁴.
 */

#include <stddef.h>   /* for size_t, included for completeness */

static seq_t triangle_value(idx_t row, idx_t col)
{
    /* Base cases */
    if (row == 1)                /* first row */
        return (seq_t)1;
    if (col == 1)                /* first column */
        return (seq_t)1;
    if (row == 2 && col == 2)    /* special entry */
        return (seq_t)2;

    /* Recursive descent following the definition */
    if (col == 2)                /* T(n,2) = T(n-1,n-2) + 1 */
        return triangle_value(row - 1, row - 2) + (seq_t)1;
    if (col == 3)                /* T(n,3) = T(n-1,n-1) + 1 */
        return triangle_value(row - 1, row - 1) + (seq_t)1;

    /* General case: T(n,k) = T(n-1,k-3) + 1 for k > 3 */
    return triangle_value(row - 1, col - 3) + (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset is 1; never reached */
        return (seq_t)0;

    /* Find the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column position within the row */
    idx_t tri_prev = r * (r - 1) / 2;
    idx_t c = n - tri_prev;      /* 1‑based column index */

    /* Return the value at (r,c) in the triangle */
    return triangle_value(r, c);
}
