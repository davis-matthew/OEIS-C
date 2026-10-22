/*
 * A384718  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A052750.
 * https://oeis.org/A384718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = k * (2*n + k)^(n-1) for n > 0 Sequence a(n) is read by
 *            antidiagonals downwards (row 0 first)
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index s so that
     * s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t before = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t offset = n - before;          /* position inside diagonal s   */
    idx_t row    = offset;              /* n in the square array       */
    idx_t col    = s - offset;          /* k in the square array       */

    if (row == 0)          /* row 0 consists of ones */
        return 1;
    if (col == 0)          /* k = 0 gives zero for n > 0 */
        return 0;

    seq_t base = (seq_t)(2 * (seq_t)row + (seq_t)col);
    seq_t exp  = (seq_t)(row - 1);

    seq_t pow_val = big_pow(base, exp);
    return pow_val * (seq_t)col;
}
