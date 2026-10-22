/*
 * A385406  Triangle read by rows: T(n, k) = n*(n+1)/2 - floor((n-1)/2) -
 *          (-1)^k * floor(k/2).
 * https://oeis.org/A385406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r, k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n - (r-1)r/2. T(r,k) = r(r+1)/2 - floor((r-1)/2) - (-1)^k *
 *            floor(k/2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r by binary search on r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_sum);   /* 1‑based column inside row r */

    /* Compute T(r,k) */
    seq_t value = (seq_t)r * ((seq_t)r + 1) / 2;           /* r(r+1)/2 */
    value -= (seq_t)((r - 1) / 2);                         /* -floor((r-1)/2) */
    seq_t flk = (seq_t)(k / 2);                            /* floor(k/2) */

    if ((k & 1) == 0)   /* k even: subtract floor(k/2) */
        value -= flk;
    else                /* k odd: add floor(k/2) */
        value += flk;

    return value;
}
