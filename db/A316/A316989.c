/*
 * A316989  Irregular triangle read by rows: row n consists of the
 *          coefficients in the expansion of the polynomial (x^2 + 4*x + 3)*(x
 *          + 1)^(2*n) + (x^2 - 1)*(x^2 + 3*x + 3).
 * https://oeis.org/A316989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^k in
 * (x^2+4x+3)*(x+1)^(2*r) + (x^2-1)*(x^2+3x+3),
 * where r is the row index and k is the column index within that row
 * obtained by flattening the irregular triangle row by row. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k from the flat index n. */
    idx_t r = 0;
    idx_t cumulative = 0;          /* number of terms before the current row */

    while (1) {
        idx_t row_len = (r == 0) ? 5 : 2 * r + 3;
        if (cumulative + row_len > n)
            break;
        cumulative += row_len;
        ++r;
    }

    idx_t k = n - cumulative;     /* column index within row r */

    /* Contribution from (x^2+4x+3)*(x+1)^(2*r) */
    const seq_t coeff_q[3] = { 3, 4, 1 };               /* [x^2, x, 1] */
    seq_t sum = 0;
    idx_t two_r = 2 * r;

    for (int j = 0; j < 3; ++j) {
        if (k >= (idx_t)j && (k - j) <= two_r) {
            seq_t bin = binom(two_r, k - j);
            sum += (seq_t)coeff_q[j] * bin;
        }
    }

    /* Add constant polynomial (x^2-1)*(x^2+3x+3) coefficients. */
    switch (k) {
    case 0:
    case 1:
        sum -= 3;   /* -3 contributes for k=0,1 */
        break;
    case 2:
        sum += 2;
        break;
    case 3:
        sum += 3;
        break;
    case 4:
        sum += 1;
        break;
    default:
        /* k > 4 : no additional contribution */
        break;
    }

    return sum;
}
