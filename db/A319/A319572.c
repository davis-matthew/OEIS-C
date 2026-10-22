/*
 * A319572  The x coordinates of the stripe enumeration of N X N where N = {0,
 *          1, 2, ...}.
 * https://oeis.org/A319572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let k = floor((sqrt(8*n + 1) - 1) / 2). Let t = k*(k+1)/2. Let d
 *            = n - t. If k is odd, a(n) = d; else a(n) = k - d. (This is
 *            equivalent to the stripe‑enumeration of an N×N grid described in
 *            A319572.)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t low = 0, high = 1;
    __int128 tri;

    /* Find an upper bound for k */
    while (1) {
        tri = (__int128)high * (high + 1) / 2;
        if (tri > n) break;
        high <<= 1;
    }

    /* Binary search for the largest k with k*(k+1)/2 <= n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;      /* use upper mid to converge */
        tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t k = low;
    idx_t t = (idx_t)(((__int128)k * (k + 1)) / 2);
    idx_t d = n - t;

    if (k & 1)
        return (seq_t)d;              /* odd k: ascending part */
    else
        return (seq_t)(k - d);        /* even k: descending part */
}
