/*
 * A278317  Number of neighbors of each new term in a right triangle read by
 *          rows.
 * https://oeis.org/A278317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 9603, u32 n <= 9603, u64 n <= 9603,
 *            u128 n <= 9603, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278317 – Number of neighbors of each new term in a right triangle.
   The triangle is indexed row‑by‑row with rows of lengths 1,2,3,… .
   For the element at (row, col) the value (number of present neighbors at
   the time of addition) is
        0  if row == 1
        1  if row == 2 && col == 1
        2  if col == 1 (row > 2)  or col == row
        3  if col == row-1
        4  otherwise (interior positions).
   This rule reproduces the OEIS sequence. */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.  */
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        /* Compute mid*(mid+1)/2 in seq_t to avoid overflow. */
        seq_t val = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (val >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the column k (1‑based) of the element n. */
    seq_t prev_sum = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t k = n - (idx_t)prev_sum;

    /* Determine the value according to the rules above. */
    if (r == 1)               return 0;           /* 0 */
    if (r == 2 && k == 1)      return 1;           /* 1 */
    if (k == 1 || k == r)      return 2;           /* 2 on the sides */
    if (k == r - 1)            return 3;           /* 3 on the second diagonal */
    return 4;                 /* 4 for interior positions */
}
