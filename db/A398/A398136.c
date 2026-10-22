/*
 * A398136  Triangle read by rows: T(n, k) = n! * n^(n - k) / (n - k)!.
 * https://oeis.org/A398136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = r! * r^(r-k) / (r-k)! , where r =
 *            ⌊(√(1+8n)-1)/2⌋ and k = n - r(r+1)/2 (triangle read by rows,
 *            indices start at 0)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       Binary search on r. */
    idx_t low = 0;
    idx_t high = n;                       /* r never exceeds n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t c = (__uint128_t)mid * (mid + 1) / 2;
        if (c <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column index k within this row. */
    __uint128_t start = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;   /* start ≤ n, so the cast is safe */

    /* Falling factorial part: r! / (r-k)! = r*(r-1)*...*(r-k+1) */
    seq_t falling = 1;
    for (idx_t i = 0; i < k; ++i) {
        falling *= (seq_t)(r - i);
    }

    /* Power part: r^(r-k) */
    seq_t power = big_pow((seq_t)r, (seq_t)(r - k));

    return falling * power;
}
