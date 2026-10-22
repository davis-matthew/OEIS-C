/*
 * A317617  Triangle T read by rows: T(n, k) = (n^3 + n)/2 + (k - (n +
 *          1)/2)*(n mod 2).
 * https://oeis.org/A317617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) read by rows, where * r = smallest integer with
 *            r*(r+1)/2 >= n, * k = n - r*(r-1)/2, * T(r,k) = (r^3 + r)/2 + (k
 *            - (r+1)/2)*(r mod 2)
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t mval = (__uint128_t)mid * (mid + 1) / 2;
        if (mval >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Previous cumulative length: sum_{i=1}^{r-1} i = r*(r-1)/2 */
    idx_t prev = (idx_t)(((__uint128_t)r * (r - 1)) / 2);
    idx_t k = n - prev;          /* 1‑based column index in row r */

    /* Base value (r^3 + r)/2 */
    seq_t rr  = (seq_t)r;
    seq_t base = rr * rr * rr;   /* r^3 */
    base += rr;                  /* r^3 + r */
    base /= 2;                   /* (r^3 + r)/2 */

    seq_t result = base;

    if (r & 1) {                 /* odd r: add column offset */
        long long diff =
            (long long)k - ((long long)r + 1) / 2;  /* k - (r+1)/2 */
        if (diff >= 0)
            result += (seq_t)diff;
        else
            result -= (seq_t)(-diff);
    }

    return result;
}
