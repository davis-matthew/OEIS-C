/*
 * A323222  A(n, k) = [x^k] (1 - 4*x)^(-n/2)*x/(1 - x), square array read by
 *          ascending antidiagonals with n >= 0 and k >= 0.
 * https://oeis.org/A323222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient sum A(n,k) over all tuples of length n *
 *            with entries in {0,…,k-1} and sum < k. Equivalently, * A(n,k) =
 *            [x^k] (1-4x)^(-n/2) * x/(1-x). The sequence a is read by *
 *            ascending antidiagonals of the array A(n,k)
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find antidiagonal index t: t(t+1)/2 <= n < (t+1)(t+2)/2 */
    idx_t t = (idx_t)((sqrt((double) (8.0 * n + 1.0)) - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((unsigned __int128) t * (t + 1) / 2 > n) --t;
    while ((unsigned __int128) (t + 1) * (t + 2) / 2 <= n) ++t;

    idx_t i = n - (unsigned __int128) t * (t + 1) / 2;  /* position in the diagonal */
    idx_t row = t - i;                                 /* n = row index in A(n,k) */
    idx_t col = i;                                     /* k = column index */

    if (col == 0) return 0;   /* [x^0] part is always 0 */

    seq_t result = 0;
    seq_t term   = 1;        /* term_0 = 1 */

    /* Sum term_j for j = 0 .. col-1:
         term_j = 2^j * prod_{u=1}^j (row + 2u - 2) / j!   (j! divides numerator) */
    result += term;           /* j = 0 */

    for (idx_t j = 1; j < col; ++j) {
        /* factor = row + 2*j - 2, guaranteed >= 0 when j >= 1 */
        unsigned __int128 part = (unsigned __int128)row + 2 * j;
        seq_t factor = (seq_t)(part - 2);   /* subtract 2 safely (>=0) */

        /* term_j = term_{j-1} * 2 * factor / j */
        term = term * (seq_t)2 * factor / j;
        result += term;
    }

    return result;
}
