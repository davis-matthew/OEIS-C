/*
 * A349736  Binomial coefficients C(m,k) such that C(m,k), C(m,k+1), C(m,k+2)
 *          with 0 <= k <= m-2 form an increasing arithmetic progression.
 * https://oeis.org/A349736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n^2+4n+2,(n^2+3n-2)/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute m = n^2 + 4n + 2 and k = (n^2 + 3n - 2)/2 */
    idx_t m = n * n + 4 * n + 2;
    idx_t k = (n * n + 3 * n - 2) / 2;
    return binom(m, k);
}
