/*
 * A365495  Square array read by ascending antidiagonals: T(n,k) is the parity
 *          of the k-th iterate of the 3x+1 function started at n, with n >= 1
 *          and k >= 0.
 * https://oeis.org/A365495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = parity of the k-th iterate of the 3x+1 function, where
 *            (n,k) is obtained by unfolding the square array T(n,k) along its
 *            ascending antidiagonals
 * Range:     u8 n <= 5747, u16 n <= 9879, u32 n <= 9879, u64 n <= 9879,
 *            u128 n <= 9879, big any n
 * Helpers:   cum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cum.h"

seq_t a(idx_t n)
{
    /* Find diagonal s with cum(s-1) < n <= cum(s). */
    idx_t low = 0, high = 1;
    while (cum(high) < n) {
        if (high > UINT64_MAX / 2) {   /* avoid overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cum(mid) < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low;

    /* Elements before diagonal s: s*(s+1)/2 */
    idx_t prev = (idx_t)(((__int128)s * (s + 1)) / 2);

    /* Position inside diagonal (0‑based). */
    idx_t offset = n - prev - 1;

    /* Corresponding row (1‑based) and column (0‑based). */
    idx_t row = s - offset + 1;   /* n = s - k + 1  */
    idx_t col = offset;           /* k = offset     */

    /* Compute parity of col‑th iterate starting from row. */
    seq_t val = (seq_t)row;
    for (idx_t i = 0; i < col; ++i) {
        if (val & 1U)                 /* odd → (3x+1)/2 */
            val = (val * 3 + 1) >> 1;
        else                          /* even → x/2  */
            val >>= 1;
    }
    return (seq_t)(val & 1U);
}
