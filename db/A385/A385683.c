/*
 * A385683  Complement of A030511.
 * https://oeis.org/A385683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+m if n+m > floor(2*(m+1)^2/3) * a(n) = n+m-1 otherwise
 *            * where m = floor(sqrt(3*(n-1)/2))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = floor(sqrt(3*(n-1)/2))   */
    unsigned __int128 n128 = n;
    unsigned __int128 x = (unsigned __int128)3 * (n128 - 1) / 2;

    /* Binary search for floor(sqrt(x)) with 64‑bit bounds */
    unsigned long long lo = 0, hi = 1;
    while ((unsigned __int128)hi * hi <= x) hi <<= 1;
    while (lo + 1 < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    uint64_t m = lo;  /* floor sqrt */

    /* Compute threshold t = floor(2*(m+1)^2/3) */
    unsigned __int128 m1 = (unsigned __int128)(m + 1);
    unsigned __int128 t = (unsigned __int128)2 * m1 * m1 / 3;

    /* Decide whether to add m or m-1 */
    unsigned __int128 sum = n128 + (unsigned __int128)m;
    if (sum > t)
        return (seq_t)sum;
    else
        return (seq_t)(sum - 1);
}
