/*
 * A332602  Tridiagonal matrix M read by antidiagonals: main diagonal is
 *          1,2,2,2,2,..., two adjacent diagonals are 1,1,1,1,1,...
 * https://oeis.org/A332602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = M[i][j], where M has main diagonal 1,2,2,… and * sub‑ and
 *            super‑diagonals equal to 1, other entries 0. * For each n, find
 *            the minimal d such that (d+1)(d+2)/2 > n. * Then p = n –
 *            d(d+1)/2, i = p, j = d – p. * Finally: * if i==j and i>0 -> 2 *
 *            else if i==j==0 -> 1 * else if |i‑j|==1 -> 1 *...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find minimal d with cumulative > n. */
    idx_t low = 0;
    idx_t high = n + 1;          /* high is surely large enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)(mid + 1) * (mid + 2) / 2; /* cumulative for d = mid */
        if (t > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                       /* minimal d with cumulative > n */

    /* Previous cumulative count (entries before diagonal d). */
    __int128 prev = (__int128)d * (d + 1) / 2; /* cumulative for d-1 */
    idx_t pos = (idx_t)(n - prev);            /* position inside diagonal d */

    idx_t i = pos;
    idx_t j = d - pos;

    if (i == j) {
        if (i == 0) return (seq_t)1;
        else        return (seq_t)2;
    }
    if ((i + 1 == j) || (i == j + 1))
        return (seq_t)1;
    return (seq_t)0;
}
