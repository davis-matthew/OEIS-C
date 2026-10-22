/*
 * A300454  Irregular triangle read by rows: row n consists of the
 *          coefficients of the expansion of the polynomial 2*(x + 1)^(n + 1)
 *          + x^3 + 2*x^2 - x - 2.
 * https://oeis.org/A300454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A300454.
   For each index n it first determines the row r and the position c
   within that row of the flattened triangle.  The triangle entry is
   the coefficient of x^c in the polynomial
          P(r;x) = 2*(x+1)^(r+1) + x^3 + 2*x^2 - x - 2.
   The coefficient is:
          T(r,c) = 2 * C(r+1, c) + [c==3] + 2*[c==2] - 1*[c==1] - 2*[c==0]
   where C(n,k) is the binomial coefficient (0 if k>n).  The term
   a(n) is just this coefficient read row‑by‑row (unordered array). */
  
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row r and column c such that the flattened index n belongs.
     * Each row r has length:
     *   - 4 for r = 0,1,2  (degrees 0..3 always present)
     *   - r+2 for r >= 3    (degrees 0..r+1 from (x+1)^(r+1))
     */
    idx_t r = 0;         /* current row */
    idx_t prev_len_sum = 0;  /* sum of lengths of rows before r */

    while (1)
    {
        idx_t row_len = (r < 3) ? 4 : r + 2;
        if (n < prev_len_sum + row_len)  /* n lies in row r */
            break;
        prev_len_sum += row_len;
        ++r;
    }

    /* Column index inside the row */
    idx_t c = n - prev_len_sum;          /* 0‑based */

    /* Compute the coefficient T(r,c) */
    seq_t binom = binomial(r + 1, c);    /* C(r+1, c), 0 if c > r+1 */
    seq_t val   = binom * 2;             /* 2 * binomial */

    /* Adjust for the extra terms x^3 + 2x^2 - x - 2 */
    if (c == 3)
        val += 1;
    else if (c == 2)
        val += 2;
    else if (c == 1)
        val -= 1;
    else if (c == 0)
        val -= 2;

    return val;
}
