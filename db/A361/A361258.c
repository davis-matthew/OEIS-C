/*
 * A361258  Irregular triangle read by rows in which row n lists the print
 *          order of a 4n-page booklet.
 * https://oeis.org/A361258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) gives the n-th term of the concatenation of rows, where row
 *            r lists, in order, the print order of a 4r‑page booklet: for i =
 *            1..r the four numbers are: 2*i, 4r - 2*i + 1, 4r - 2*i + 2, 2*i
 *            - 1. The sequence is the concatenation of all such rows,
 *            1-indexed
 * Range:     u8 n <= 8066, u16 n <= 9608, u32 n <= 9608, u64 n <= 9608,
 *            u128 n <= 9608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that 2*r*(r+1) >= n and 2*(r-1)*r < n.
       Use exponential search + binary search with 128‑bit arithmetic
       to avoid overflow. */
    idx_t lo = 0, hi = 1;
    while ((((unsigned __int128)hi) * (hi + 1u) * 2u) < n)
        hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((((unsigned __int128)mid) * (mid + 1u) * 2u) >= n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t r = hi;

    /* Previous total number of terms before row r */
    idx_t prev = 0;
    if (r > 1u)
        prev = 2 * (r - 1u) * r;

    /* Position of n within row r (1‑based) */
    idx_t pos = n - prev;
    /* 0‑based index within the 4*r numbers */
    idx_t local = (pos - 1u) % 4u;
    /* Sheet number i (1‑based) */
    idx_t i = (pos - 1u) / 4u + 1u;

    /* Compute the required value using seq_t arithmetic */
    switch (local) {
        case 0u:
            return (seq_t)(2u * i);
        case 1u:
            return (seq_t)(4u * r - 2u * i + 1u);
        case 2u:
            return (seq_t)(4u * r - 2u * i + 2u);
        case 3u:
            return (seq_t)(2u * i - 1u);
        default:
            /* Should never happen */
            return (seq_t)0;
    }
}
