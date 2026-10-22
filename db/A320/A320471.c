/*
 * A320471  a(n) = floor(sqrt(n)) mod ceiling(sqrt(n)).
 * https://oeis.org/A320471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n)) mod ceil(sqrt(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = 0 if n is a perfect square,       */
/* otherwise a(n) = floor(sqrt(n)).                    */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for an unsigned 64‑bit integer */
static idx_t
isqrt_u64(idx_t x)
{
    /* Binary search bounds: 0 <= result <= 2^32, since (2^32)^2 = 2^64 > max x */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1) << 32;   /* 2^32 */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;
}

/* OEIS A320471: a(n) = floor(sqrt(n)) mod ceil(sqrt(n)) */
seq_t a(idx_t n)
{
    idx_t m = isqrt_u64(n);
    if ((unsigned __int128)m * m == n)
        return (seq_t)0;
    return (seq_t)m;
}
