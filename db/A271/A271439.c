/*
 * A271439  If n is a triangular number, a(n) = 0, otherwise a(n) = n -
 *          A002024(n) + 1.
 * https://oeis.org/A271439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if n is triangular; otherwise a(n) equals the number
 *            immediately above n in the triangular array of the naturals
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* 0 belongs to the top row, so its answer is 0. */
    if (n == 0)
        return 0;

    /* Find diagonal d such that T(d) < n <= T(d+1),
       where T(k) = k*(k+1)/2.  D is the sum of the row and column indices
       of n.  The initial estimate comes from the quadratic formula. */
    long double sq = sqrtl((long double)8 * n + 1);
    idx_t d = (idx_t)((sq - 1.0L) / 2.0L);

    /* Correct any rounding errors due to floating‑point arithmetic. */
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 < n)
        ++d;
    while (d > 0 && (unsigned __int128)d * (d + 1) / 2 >= n)
        --d;

    /* i is the distance from the top of the diagonal:
       n = T(d+1) - i   (0 <= i <= d). */
    unsigned __int128 Td1 = (unsigned __int128)(d + 1) * (d + 2) / 2;
    idx_t i = (idx_t)(Td1 - n);

    if (i == 0)   /* n is triangular (top of its diagonal) */
        return 0;

    /* The element directly above n lies on diagonal d-1
       and is at distance i-1 from its top.  Its value is
       T(d) - (i-1). */
    unsigned __int128 Td = (unsigned __int128)d * (d + 1) / 2;
    seq_t above = (seq_t)(Td - (i - 1));
    return above;
}
