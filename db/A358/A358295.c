/*
 * A358295  Triangle read by rows: T(n,k) = n^3*k^3*(n+k)^2, n>=1, 1 <= k <=
 *          n.
 * https://oeis.org/A358295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r^3 * k^3 * (r + k)^2 where r is the minimal integer with
 *            r(r+1)/2 >= n and k = n - (r-1)r/2. This yields the nth term of
 *            the triangle T(n,k) = n^3 k^3 (n+k)^2 read row‑wise
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Upper bound for r: ceil((sqrt(2*n)+2)) guarantees r(r+1)/2 >= n. */
    idx_t low  = 1;
    idx_t high = (idx_t)(sqrt((double)(2ULL * n)) + 2);

    /* Binary search for the smallest r such that r(r+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);

        /* Use 128‑bit arithmetic to avoid overflow in mid*(mid+1)/2. */
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r   = low;                           /* row index (1‑based)   */
    idx_t prev = (r - 1) * r / 2;              /* count of all previous rows */
    idx_t k   = n - prev;                      /* column index (1‑based) */

    seq_t r3 = (seq_t)r * r;
    r3 *= r;                                   /* r^3 */
    seq_t k3 = (seq_t)k * k;
    k3 *= k;                                   /* k^3 */
    seq_t sum = (seq_t)(r + k);                /* r + k */
    seq_t sum2 = sum * sum;                    /* (r + k)^2 */

    seq_t val = r3 * k3;                       /* r^3 * k^3 */
    val *= sum2;                               /* * (r + k)^2 */

    return val;
}
