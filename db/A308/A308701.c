/*
 * A308701  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(k*(d-1)).
 * https://oeis.org/A308701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{d | m} d^(k*(d-1)), where (m,k) are obtained from
 *            the index n by reading the square array A(n,k) along
 *            antidiagonals with n ascending on each diagonal
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 40, u64 n <= 60, u128 n <= 111,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that
       s*(s+1)/2 >= n > (s-1)*s/2.  */
    idx_t idx = n;
    idx_t s = (idx_t)((sqrt(8.0 * (double)idx + 1.0) - 1.0) / 2.0);
    while ((unsigned __int128)s * (s + 1) / 2 < idx) ++s;
    while (s > 1 &&
           (unsigned __int128)(s - 1) * s / 2 >= idx) --s;

    idx_t prev = (s - 1) * s / 2;          /* total elements before diagonal s */
    idx_t p    = idx - prev;               /* position on this diagonal (1‑based) */
    idx_t row  = p;                        /* n in A(n,k) */
    idx_t k    = s - p;                    /* k in A(n,k) */

    seq_t result = 0;
    for (idx_t d = 1; d <= row; ++d)
    {
        if (row % d == 0)
        {
            seq_t base     = (seq_t)d;
            seq_t exponent = (seq_t)k * (seq_t)(d - 1);
            result += big_pow(base, exponent);
        }
    }
    return result;
}
