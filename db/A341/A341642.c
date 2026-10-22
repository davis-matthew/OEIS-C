/*
 * A341642  Number of strictly superior prime divisors of n.
 * https://oeis.org/A341642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of distinct prime divisors p of n with p^2 > n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1;
    idx_t hi = n >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t sqrt_n = isqrt_u64(n);
    idx_t temp = n;
    seq_t count = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            if (p > sqrt_n) {
                count += (seq_t)1;
            }
            while (temp % p == 0)
                temp /= p;
        }
    }

    if (temp > 1 && temp > sqrt_n)
        count += (seq_t)1;

    return count;
}
