/*
 * A356356  Triangle of number of rectangles in the interior of the rectangle
 *          with vertices (k,0), (0,k), (n,n+k) and (n+k,n), read by rows.
 * https://oeis.org/A356356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 169, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A356356: Triangle of number of rectangles in the interior of
   the rectangle with vertices (k,0),(0,k),(n,n+k),(n+k,n),
   read by rows.
   For the flattened sequence a(n) (offset 1) we first find the
   row r and column c such that n lies in the r‑th row of the
   triangle:
        r = ceil((sqrt(1+8n) – 1)/2)
        c = n – r*(r‑1)/2
   Then T(r,c) is given by the explicit formula
        T(r,c) = ((r‑c)*(4*c^3 – c))/3 + (4*c^4 – c^2 – 3*c)/6
   This value is returned.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n.  */
    idx_t low = 1;
    idx_t high = n;                 /* n is safe upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * (seq_t)(mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                            /* row number */
    seq_t prev = (seq_t)(r - 1) * (seq_t)r / 2;/* number of elements before this row */
    idx_t c = (idx_t)((seq_t)n - prev);       /* column number in the row */

    /* Compute T(r,c) using the explicit integer formula.  */
    seq_t r_s = (seq_t)r;
    seq_t c_s = (seq_t)c;
    seq_t r_minus_c = r_s - c_s;
    seq_t c_sq = c_s * c_s;
    seq_t c_cube = c_sq * c_s;
    seq_t c_fourth = c_cube * c_s;

    /* First term: ((r-c)*(4*c^3 - c))/3 */
    seq_t term1_num = r_minus_c * ((seq_t)4 * c_cube - c_s);
    seq_t term1 = term1_num / 3;

    /* Second term: (4*c^4 - c^2 - 3*c)/6 */
    seq_t term2_num = ((seq_t)4 * c_fourth - c_sq - (seq_t)3 * c_s);
    seq_t term2 = term2_num / 6;

    return term1 + term2;
}
