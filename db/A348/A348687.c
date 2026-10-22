/*
 * A348687  Triangle read by rows, T(n, k) = n - HammingWeight(k), for 0 <= k
 *          <= n.
 * https://oeis.org/A348687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r - popcount(k) where * r = ⌊(√(1+8n)-1)/2⌋ (row index) *
 *            k = n - r(r+1)/2 (column index) * T(r,k) = r - popcount(k) is
 *            the triangle entry. * * The sequence is the triangle read row by
 *            row (flattened)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* popcount helper – available as builtin in GCC/Clang */
#define POPCOUNT(x) __builtin_popcountll((unsigned long long)(x))

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 start = (__int128)mid * (mid + 1) / 2;
        if (start > n) {
            high = mid - 1;
        } else {
            /* Within the row if n <= start + mid */
            if ((__int128)start + mid >= n) {
                r = mid;
                break;
            }
            low = mid + 1;
        }
    }

    /* Compute column index k */
    __int128 start = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* Triangle value: r - popcount(k) */
    return (seq_t)(r - POPCOUNT(k));
}
