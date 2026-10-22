/*
 * A395604  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^j *
 *          binomial(k,j) * (4-j)^n.
 * https://oeis.org/A395604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r and k are the row and column * of n in the
 *            triangle read by rows. The triangle * is defined by * T(r,k) =
 *            Σ_{j=0..k} (-1)^j * binomial(k,j) * (4-j)^r * with r,k≥0. We
 *            determine r as the smallest integer * such that (r+1)(r+2)/2 >
 *            n; then k = n - r(r+1)/2
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Determine the row r that contains the n‑th element. */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within that row. */
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* Compute T(r,k) = Σ_{j=0..k} (-1)^j binomial(k,j) (4-j)^r. */
    seq_t result = 0;
    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t binom = binomial(k, j);

        /* Base (4-j) may become negative for j>4.  Perform the
         * subtraction in signed seq_t to preserve the sign. */
        seq_t base = (seq_t)4 - (seq_t)j;

        seq_t pow   = big_pow(base, (seq_t)r);
        seq_t term  = binom * pow;

        if (j & 1)
            result -= term;
        else
            result += term;
    }

    return result;
}
