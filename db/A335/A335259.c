/*
 * A335259  Triangle read by rows: T(n,k) = k^ceiling(n/k) for 1 <= k <= n.
 * https://oeis.org/A335259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k^ceil(r/k) where r is the unique positive integer with
 *            r*(r+1)/2 ≥ n > (r-1)*r/2 and k = n - (r-1)*r/2. Here
 *            T(r,k)=k^ceil(r/k) is the triangle entry, and a(n) is the
 *            flattening of the triangle row‑by‑row
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined offset, but return 0 for safety */

    /* Find the row r */
    idx_t r = 1;
    while ( ((unsigned __int128)r * (r + 1)) / 2 < n ) {
        ++r;
    }

    /* Column within that row */
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2;   /* total numbers before row r */
    idx_t k = n - prev;                                 /* 1 ≤ k ≤ r */

    /* Ceiling of r/k */
    idx_t e = (r + k - 1) / k;

    /* Compute k^e as seq_t */
    return pow_uint((uint64_t)k, e);
}
