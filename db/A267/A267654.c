/*
 * A267654  Irregular triangle of palindromic subsequences. Every row has
 *          2*n+1 terms. From the second row, there are only two alternated
 *          numbers: 2*n+4 and 2*n+2.
 * https://oeis.org/A267654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2 * A086520(n+2). Even indices: a(2k) = 4*m + 2 where m =
 *            ⌊√(k/2)⌋. Odd indices: a(2k+1) = 4*t where t is the least
 *            integer with 2*t*(t+1) > k
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t v)
{
    /* Integer square root for 64‑bit values. */
    idx_t low = 0, high = 1ULL << 32;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * mid <= v)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

static inline idx_t isqrt128(__int128 v)
{
    /* Integer square root for a 128‑bit value, result fits in idx_t. */
    idx_t low = 0, high = (idx_t)-1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * mid <= v)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                 /* even index: a(2k)  */
        idx_t k = n >> 1;                /* k = n/2  */
        /* m = floor( sqrt(k/2) )  */
        idx_t half = k >> 1;             /* floor(k/2) */
        idx_t m = isqrt_u64(half);
        return (seq_t)(4 * m + 2);
    } else {                             /* odd index: a(2k+1) */
        idx_t k = (n - 1) >> 1;          /* k = (n-1)/2  */
        /* Compute t minimal such that 2*t*(t+1) > k.  */
        __int128 val = (__int128)1 + (__int128)2 * k;   /* 1 + 2k  */
        idx_t sqrt_val = isqrt128(val);
        idx_t t = (sqrt_val - 1) >> 1;   /* (sqrt_val - 1)/2 */
        while ((__int128)2 * t * (t + 1) <= k)
            ++t;
        return (seq_t)(4 * t);
    }
}
