/*
 * A368942  Burning number of the n-antiprism graph.
 * https://oeis.org/A368942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((sqrt(16*n - 1) + 1)/4) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned __int128 value.
   Returns the largest idx_t such that s*s <= x. */
static inline idx_t isqrt_u128(__int128 x)
{
    __int128 low  = 0;
    __int128 high = ((__int128)1 << 64);   /* 2^64, greater than any sqrt needed */

    while (low < high) {
        __int128 mid = (low + high) >> 1;
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        /* avoid overflow: mid*mid <= x  <=>  mid <= x / mid */
        if (mid > x / mid)
            high = mid;
        else
            low  = mid + 1;
    }
    return (idx_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety return 0 if n==0. */
    if (n == 0) return (seq_t)0;

    __int128 t = (__int128)16 * n - 1;     /* 16*n - 1, no overflow */
    idx_t     s = isqrt_u128(t);           /* floor(sqrt(t))       */
    idx_t     res = (s + 1) / 4 + 1;        /* floor((s+1)/4)+1     */
    return (seq_t)res;
}
