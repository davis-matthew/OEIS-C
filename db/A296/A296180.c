/*
 * A296180  Triangle read by rows: T(n, k) = 3*(n - k)*k + 1, n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A296180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 3*(r - k)*k + 1, where r = floor((sqrt(8n+1)-1)/2), k = n
 *            - r*(r+1)/2, and T(r,k)=3*(r-k)*k+1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);           /* upper bound for any 64‑bit n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* mid biased up */
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    __int128 start128 = (__int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;

    seq_t t1 = (seq_t)(r - k);
    seq_t t2 = (seq_t)k;
    return 3 * (t1 * t2) + 1;
}
