/*
 * A343044  Array T(n, k), n, k >= 0, read by antidiagonals; lunar addition
 *          table for the primorial base.
 * https://oeis.org/A343044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = lunar addition of the antidiagonal pair (r,s) in
 *            primorial base * where (r,s) are defined by * * m = floor(
 *            (sqrt(8*n+1)-1) / 2 ), * offset = n - m*(m+1)/2, * r = m -
 *            offset, * s = offset. * The lunar addition takes the digitwise
 *            maximum in primorial base (bases 2,3,5,7,…) * and returns th...
 * Range:     u8 n <= 9326, u16 n <= 9326, u32 n <= 9326, u64 n <= 9326,
 *            u128 n <= 9326, big any n
 * Helpers:   primorial_lunar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/primorial_lunar.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    while ((unsigned __int128)(m + 1) * (m + 2) / 2 <= n)
        ++m;

    /* Offset on the antidiagonal. */
    idx_t offset = n - (unsigned __int128)m * (m + 1) / 2;

    /* Convert to antidiagonal coordinates (r,s). */
    idx_t r = m - offset;
    idx_t s = offset;

    return primorial_lunar(r, s);
}
