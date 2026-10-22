/*
 * A329829  Beatty sequence for (2+sqrt(10))/3.
 * https://oeis.org/A329829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(2+sqrt(10))/3 )
 * Range:     u8 n <= 49, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 128‑bit values */
static unsigned __int128 isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 (offset 1). */
    seq_t n_seq = (seq_t)n;

    /* Compute floor(sqrt(10 * n^2)).  The product uses 128‑bit arithmetic
       to avoid overflow for all 64‑bit n that fit in the result type. */
    unsigned __int128 tmp = (unsigned __int128)n * (unsigned __int128)n * (unsigned __int128)10;
    unsigned __int128 sqrt_term128 = isqrt_uint128(tmp);
    seq_t sqrt_term_seq = (seq_t)sqrt_term128;

    /* a(n) = (2*n + floor(n*sqrt(10))) / 3 */
    seq_t two_n = n_seq + n_seq;
    seq_t sum = two_n + sqrt_term_seq;
    return sum / (seq_t)3;
}
