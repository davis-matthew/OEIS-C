/*
 * A399500  Triangle read by rows: row n gives the n consecutive integers
 *          centered at n, excluding n if n is even.
 * https://oeis.org/A399500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the nth element of the triangle read by rows defined by
 *            OEIS * A399500. * Row r (r ≥ 1) contains r integers: * • if r =
 *            2m+1 (odd), row r = {m+1, … , 3m+1} * • if r = 2m (even), row r
 *            = {m, … , 2m‑1} ∪ {2m+1, … , 3m} * The global index n determines
 *            the row r such that r(r‑1)/2 < n ≤ r(r+1...
 * Range:     u8 n <= 9567, u16 n <= 9567, u32 n <= 9567, u64 n <= 9567,
 *            u128 n <= 9567, big any n
 * Helpers:   cumsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cumsum.h"

seq_t a(idx_t n) {
    /* binary search for row r: smallest r with r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cumsum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    /* previous cumulative sum */
    unsigned __int128 prev = cumsum(r - 1);
    idx_t prev_sum = (idx_t)prev;          /* < n by construction */
    idx_t pos = n - prev_sum - 1;          /* 0‑based position in row r */

    unsigned __int128 val128;
    if (r & 1U) {                          /* r is odd: r = 2m+1 */
        idx_t m = (r - 1) >> 1;
        val128 = (unsigned __int128)m + 1 + pos;
    } else {                               /* r is even: r = 2m */
        idx_t m = r >> 1;
        if (pos < m)
            val128 = (unsigned __int128)m + pos;
        else
            val128 = (unsigned __int128)(2 * m) + 1 + (pos - m);
    }

    return (seq_t)val128;
}
