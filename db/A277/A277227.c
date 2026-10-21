/*
 * A277227  Triangular array T read by rows: T(n,k) gives the additive orders
 *          k modulo n, for k = 0,1, ..., n-1.
 * https://oeis.org/A277227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = row / gcd(row, col) where row is the smallest integer r
 *            such that r(r+1)/2 ≥ n, col = n - (r−1)r/2 − 1 (0 ≤ col < r)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but keep graceful */

    /* Find the row `row` such that row*(row+1)/2 ≥ n. */
    idx_t low = 1, high = n;   /* row ≤ n and ≥ 1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Compute mid*(mid+1)/2 using 128‑bit arithmetic to avoid overflow. */
        __int128 sum = (__int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    /* Compute column position within this row (0‑based). */
    __int128 prev = (__int128)(row - 1) * row / 2;
    idx_t col = (idx_t)(n - prev - 1);

    /* Compute the additive order: row / gcd(row, col). */
    seq_t sn = (seq_t)row;
    seq_t sc = (seq_t)col;
    seq_t g  = gcd(sn, sc);
    return sn / g;
}
