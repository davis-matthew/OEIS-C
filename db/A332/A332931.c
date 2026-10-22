/*
 * A332931  Sum of round(sqrt(d)) where d runs through the divisors of n.
 * https://oeis.org/A332931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} round( sqrt(d) )
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for a uint64_t.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);          /* sqrt(UINT64_MAX) <= 2^32-1 */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Rounded sqrt of a divisors: round( sqrt(d) ).
   For integer d, round(sqrt(d)) = floor(sqrt(d)) unless d > r^2 + r,
   where r = floor(sqrt(d)). */
static inline seq_t round_sqrt_u64(idx_t d)
{
    idx_t r = floor_sqrt_u64(d);
    unsigned __int128 mid = (unsigned __int128)r * r + r;
    if ((unsigned __int128)d <= mid)
        return (seq_t)r;
    else
        return (seq_t)(r + 1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t lim = floor_sqrt_u64(n);
    for (idx_t d = 1; d <= lim; ++d) {
        if (n % d == 0) {
            sum += round_sqrt_u64(d);
            idx_t partner = n / d;
            if (partner != d)
                sum += round_sqrt_u64(partner);
        }
    }
    return sum;
}
