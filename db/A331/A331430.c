/*
 * A331430  Triangle read by rows: T(n, k) =
 *          (-1)^(k+1)*binomial(n,k)*binomial(n+k,k) (n >= k >= 0).
 * https://oeis.org/A331430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(c+1) * binomial(r,c) * binomial(r+c,c) where r =
 *            floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2 (flattened
 *            triangle T(n,k) as defined in A331430)
 * Range:     i8 n <= 12, i16 n <= 39, i32 n <= 127, i64 n <= 421,
 *            i128 n <= 1325, big any n
 * Helpers:   _binomial_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_binomial_m.h"

seq_t a(idx_t n)
{
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    for (;;) {
        idx_t next = r + 1;
        unsigned __int128 tr = (unsigned __int128)next * (next + 1) / 2;
        if (tr > n) break;
        r = next;
    }

    /* column index within that row */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)tri;

    /* compute binomial factors and product */
    seq_t val = _binomial_m(r, c);
    val = val * _binomial_m(r + c, c);

    /* apply alternating sign (-1)^(c+1) */
    if ((c & 1) == 0)            /* c even → sign -1 */
        val = -val;

    return val;
}
