/*
 * A350269  Triangle read by rows, T(n, k) = (n - k)!*(n - 1)^k, for 0 <= k <=
 *          n.
 * https://oeis.org/A350269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (r-k)! * (r-1)^k, where r is the row index such that
   n < (r+1)(r+2)/2 and k = n - r(r+1)/2.  The triangle T(r,k)
   is read row‑wise starting with row 0. */

#include <stdint.h>

static inline idx_t cumulative(idx_t r)
{
    /* number of elements up to and including row r */
    return (idx_t)((__uint128_t)(r + 1) * (r + 2) / 2);
}

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative(r‑1) <= n < cumulative(r) */
    idx_t low = 0, high = 1;
    while (cumulative(high) <= n)
        high <<= 1;                 /* grow high until it exceeds n */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cumulative(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                   /* row index */

    idx_t prev = r * (r + 1) / 2;    /* cumulative up to row r‑1 */
    idx_t k = n - prev;              /* position within row r */

    /* r == 0 is a special case: value is 1 */
    if (r == 0)
        return (seq_t)1;

    /* factorial part: (r-k)! */
    seq_t fact = 1;
    idx_t m = r - k;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* power part: (r-1)^k */
    seq_t pow = 1;
    if (r == 1) {                    /* base is 0; 0^0 = 1, otherwise 0 */
        pow = (k == 0) ? (seq_t)1 : (seq_t)0;
    } else {
        seq_t base = (seq_t)(r - 1);
        for (idx_t i = 0; i < k; ++i)
            pow *= base;
    }

    return fact * pow;
}
