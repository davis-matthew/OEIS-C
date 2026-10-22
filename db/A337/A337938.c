/*
 * A337938  Irregular triangle read by rows: T(n, k) gives the primitive
 *          period of the sequence {k (Modd n)}_{k >= 0}, for n >= 1.
 * https://oeis.org/A337938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the flattened values of k (Modd r) where r is the row
 *            containing the n‑th term (rows start at 1), k is the zero‑based
 *            position in that row, q = k / r, and the Modd operation is k
 *            (Modd r) = (q even) ? (k % r) : ((r - (k % r)) % r)
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only for 'bool', not used otherwise */

seq_t a(idx_t n)
{
    /* handle the first term explicitly because the length formula for
       row 1 would underflow */
    if (n == 1U) {
        return (seq_t)0;
    }

    /* find the row r (r >= 2) such that L(r-1) < n <= L(r),
       where L(r) = r*(r+1)-3 for r >= 2 */
    idx_t low  = 2U;
    idx_t high = 2U;

    while (true) {
        unsigned __int128 cum = (unsigned __int128)high * (high + 1) - 3U;
        if (cum >= n) {
            break;
        }
        /* avoid overflow on double; high will not exceed 2^63-1
           because cum grows as ~high^2 */
        high <<= 1U;
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2U;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) - 3U;
        if (cum >= n) {
            high = mid;
        } else {
            low = mid + 1U;
        }
    }

    idx_t r = low;   /* row index (>= 2) */

    /* cumulative length before row r */
    unsigned __int128 cum_prev;
    if (r == 2U) {
        cum_prev = 1U;
    } else {
        cum_prev = (unsigned __int128)(r - 1U) * r - 3U;   /* = L(r-1) */
    }

    /* zero‑based index k in this row */
    idx_t k = n - (idx_t)cum_prev - 1U;

    /* compute k (Modd r) */
    idx_t m = k % r;
    idx_t q = k / r;
    seq_t res = (q & 1U) ? ((r - m) % r) : m;
    return res;
}
