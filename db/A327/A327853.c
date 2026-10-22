/*
 * A327853  Triangle read by rows, Sierpinski's gasket, A047999 *
 *          (0,1,2,3,4,...) diagonalized.
 * https://oeis.org/A327853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, hear, look
 * Formula:   a(n) = k * (binomial(row, k) mod 2) read by rows where row
 *            satisfies (row+1)(row+2)/2 ≥ n > row(row+1)/2, and k = n -
 *            row(row+1)/2 - 1
 * Range:     u8 n <= 9584, u16 n <= 9584, u32 n <= 9584, u64 n <= 9584,
 *            u128 n <= 9584, big any n
 * Helpers:   cum_including
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cum_including.h"

/* cumulative number of terms before row r (i.e. up to row r-1):
   r(r+1)/2. */
static inline __uint128_t cum_before(idx_t r)
{
    return (__uint128_t)r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used in OEIS */

    /* find the row r such that cum_including(r) ≥ n and
       cum_including(r-1) < n.  Binary search on r. */
    idx_t low = 0, high = 1;
    while (cum_including(high) < (__uint128_t)n)
        high <<= 1;    /* grow high until it is large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_including(mid) >= (__uint128_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k is the position within the row (0‑based). */
    idx_t k = n - (idx_t)cum_before(r) - 1;

    /* binomial(r, k) is odd iff (k & (r - k)) == 0  (Lucas' theorem). */
    bool odd = ((k & (r - k)) == 0);

    return odd ? (seq_t)k : (seq_t)0;
}
