/*
 * A334169  a(n) is the number of ON-cells in the n-th full level of ON-cells
 *          of a triangular wedge in the hexagonal grid of A151723 (after 2^k
 *          >= n generations have been computed).
 * https://oeis.org/A334169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1; a(1) = 2; For n ≥ 2: let k = floor((3 + sqrt(1 + 8*(n
 *            - 2))) / 2) let j = n - 2 - (k-2)*(k-1)/2 then a(n) = 2 +
 *            sum_{i=0}^{j} 2^(k-i-1) (equivalently a(n) = 2 + 2^k –
 *            2^(k-j-1))
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square‑root for a 128‑bit value, returning the floor. */
static inline uint64_t isqrt_uint128(__uint128_t x)
{
    uint64_t low = 0;
    uint64_t high = ~((uint64_t)0);   /* 2^64-1 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;     /* floor sqrt */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Compute k from the triangular inequality. */
    __uint128_t t = (__uint128_t)1 + (__uint128_t)8 * ((idx_t)n - 2);
    uint64_t sqrt_t = isqrt_uint128(t);   /* floor sqrt(1 + 8*(n-2)) */
    uint64_t k = (3 + sqrt_t) / 2;        /* floor division */

    /* Compute j = n - 2 - (k-2)*(k-1)/2. */
    __uint128_t tri_k = (__uint128_t)(k - 2) * (k - 1) / 2;
    __uint128_t nm2 = (__uint128_t)n - 2;
    uint64_t j = (uint64_t)(nm2 - tri_k);

    /* a(n) = 2 + 2^k – 2^(k-j-1). */
    seq_t powk   = pow2(k);
    seq_t powk2  = pow2(k - j - 1);
    seq_t ans = powk;
    ans += (seq_t)2;
    ans -= powk2;
    return ans;
}
