/*
 * A327321  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial p(x,n) constructed as in Comments; these polynomials
 *          form a strong divisibility sequence.
 * https://oeis.org/A327321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327321: coefficients of the polynomials p(x,n)
   defined by
     p(x,n) = ((sqrt(3)x+sqrt(3))^n - (sqrt(3)x-1/sqrt(3))^n) / k_n,
   where k_n is the greatest common divisor of the two terms.
   The triangle is read row‑by‑row, with row r containing r coefficients:
     a(T_{r-1}+j) = e_j / g_r     (0 ≤ j < r)
   where
     e_j = C(r,j) * (3^r - (-1)^(r-j) * 3^j),
   g_r = gcd(e_0, e_1, …, e_{r-1}).
   The global index n (starting at 1) is converted to row r by
     r(r+1)/2 ≥ n.
*/

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

/* Return the n-th term of A327321 (1‑based index) */
seq_t a(idx_t n)
{
    /* 1. Find the row r and position pos within that row */
    idx_t r = 1;                 /* current row */
    idx_t tri = 1;               /* triangular number T_r = r(r+1)/2 */
    while (tri < n) {
        r++;
        tri += r;                /* now tri = T_r */
    }
    /* start index of row r is T_{r-1} = (r-1)r/2 */
    idx_t start = (r - 1) * r / 2;
    idx_t pos = n - start - 1;   /* 0‑based position in row */

    /* 2. Pre‑compute 3^r */
    seq_t pow3_r = pow_seq((seq_t)3, r);

    /* 3. Iterate over j = 0 … r-1, computing e_j and the gcd */
    seq_t binom = 1;             /* C(r,0) */
    seq_t pow3_j = 1;            /* 3^j */
    seq_t g = 0;                 /* gcd accumulator, 0 indicates unset */
    seq_t e_pos = 0;             /* store the e value at position pos */

    for (idx_t j = 0; j < r; ++j)
    {
        /* term = 3^r - (-1)^(r-j) * 3^j */
        seq_t term = ( ((r - j) & 1) == 0 )
                     ? (pow3_r - pow3_j)   /* even exponent: subtract */
                     : (pow3_r + pow3_j);  /* odd exponent: add  */

        /* e_j = C(r,j) * term */
        seq_t e = binom * term;

        /* Update gcd */
        if (g == 0)
            g = e;
        else
            g = gcd(g, e);

        /* Store e if this is the requested position */
        if (j == pos)
            e_pos = e;

        /* Prepare for next j */
        if (j + 1 < r)
        {
            /* Next binomial coefficient: C(r,j+1) = C(r,j) * (r-j)/(j+1) */
            binom = binom * (seq_t)(r - j) / (seq_t)(j + 1);

            /* Next power of 3 */
            pow3_j = pow3_j * (seq_t)3;
        }
    }

    /* 4. Result is e_pos divided by the gcd g (exact division) */
    return e_pos / g;
}
