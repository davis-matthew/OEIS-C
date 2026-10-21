/*
 * A280026  Fill an infinite square array by following a spiral around the
 *          origin; in the n-th cell, enter the number of earlier cells that
 *          can be seen from that cell.
 * https://oeis.org/A280026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=0; for n>0, a(n)=∑_{i=1..n}d(i) where d(i)=1 unless i=k² or
 *            i=k(k+1) (k=⌊√i⌋), in which case d(i)=2−k
 * Range:     u8 n <= 7480, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for a 64‑bit unsigned integer using binary search. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) {
            return mid;
        }
        if (sq < x) {
            lo = mid + 1;
        } else {
            /* sq > x; since mid>0, hi can be reduced */
            if (mid == 0)
                return 0;
            hi = mid - 1;
        }
    }
    /* lo is the smallest integer > sqrt(x) */
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Use a signed integer type large enough for the sum.
       For n up to 10 000 this is far more than enough,
       and the arithmetic stays correct for larger values
       as the ask is only for correct behaviour on the source
       type (seq_t). */
    long long sum = 0;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t k = floor_sqrt_u64(i);
        unsigned __int128 ksq   = (unsigned __int128)k * k;
        unsigned __int128 kk1   = (unsigned __int128)k * (k + 1);

        int d = 1;
        if (ksq == i || kk1 == i) {
            d = 2 - (int)k;
        }
        sum += d;
    }

    /* The accumulated sum never becomes negative for the
       sequence, so casting to seq_t is safe. */
    return (seq_t)sum;
}
