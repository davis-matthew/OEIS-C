/*
 * A284625  Positions of 2 in A284749.
 * https://oeis.org/A284625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n*phi) - n + 2
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here phi = (1 + sqrt(5)) / 2. We compute floor(n*phi) exactly
 * by integer arithmetic: floor(n*phi) = floor( (n + floor(n*sqrt(5))) / 2 ).
 * We evaluate floor(n*sqrt(5)) as floor(sqrt(5*n*n)) using an
 * integer square‑root routine that works on 128‑bit integers.
 */

#include <stdint.h>

static inline __int128 isqrt128(__int128 x)
{
    /* Binary search for floor(sqrt(x)) in unsigned __int128 */
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 63) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == (unsigned __int128)x) return (__int128)mid;
        if (sq < (unsigned __int128)x) {
            low = mid + 1;
            __int128 rval = mid;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (__int128)(low - 1);
}

seq_t a(idx_t n)
{
    /* Convert the index to a 128‑bit integer to avoid overflow. */
    __int128 nn = (unsigned __int128)n;
    __int128 temp = nn * nn * 5;          /* 5 * n² */
    __int128 sqrt_term = isqrt128(temp);   /* floor(n * sqrt(5)) */
    __int128 floor_phi = (nn + sqrt_term) >> 1;   /* floor(n*phi) */

    __int128 result = (floor_phi << 1) - nn + 2;  /* 2*floor_phi - n + 2 */

    return (seq_t)result;
}
