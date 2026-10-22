/*
 * A380128  Triangle read by rows: Riordan array (1/(C(x)*sqrt(1-4*x)),
 *          x/C(x)) where C(x) is g.f. of A000108.
 * https://oeis.org/A380128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(m,c) where m = floor((sqrt(8*n+1)-1)/2) and c = n -
 *            m*(m+1)/2; T(m,c) = Σ_{i=0}^{m-c} (-1)^i * binomial(2*m-2*c,
 *            m-c-i) * binomial(c+i, i). The triangle T(n,k) is read
 *            row‑by‑row to form the sequence
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row m such that m*(m+1)/2 ≤ n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    /* Use a loop because n≤10^4 in tests, giving ≤141 iterations. */
    while (((__int128)(m + 1) * (m + 2) / 2) <= n)
        ++m;

    /* Column c within the row. */
    idx_t row_start = ((idx_t)((__int128)m * (m + 1) / 2));
    idx_t c = n - row_start;

    /* Compute the entry T(m,c) via the conjectured binomial formula. */
    seq_t sum = 0;
    idx_t max_i = m - c;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t n1 = 2 * m - 2 * c;          /* n1 ≥ 0 */
        idx_t k1 = m - c - i;
        idx_t n2 = c + i;
        idx_t k2 = i;

        seq_t term = binomial(n1, k1) * binomial(n2, k2);
        if (i & 1u)
            term = -term;
        sum += term;
    }
    return sum;
}
