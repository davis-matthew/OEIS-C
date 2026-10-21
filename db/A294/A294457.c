/*
 * A294457  Sum of all the diagonals of the distinct rectangles that can be
 *          made with positive integer sides such that L + W = n, W < L. The
 *          sums are then rounded to the nearest integer.
 * https://oeis.org/A294457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(2 * Sum_{i=1..floor((n-1)/2)} sqrt(i^2 + (n-i)^2))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t sqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = 1;
    /* Find an upper bound for the square root */
    while (high * high <= x)
        high <<= 1;
    /* Binary search on [low, high) */
    while (high - low > 1) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return (seq_t)low;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Use a fixed-point scaling that guarantees <0.5 rounding error.
     * k = 15 gives 2^k = 32768; error per term < 1/2^k,
     * total error < (n/2)/2^k < 0.5 for n up to at least 10000. */
    const unsigned K    = 15;
    const unsigned SHIFT = 2 * K;          // 30 for K=15
    const unsigned __int128 ROUND_ADJ = (unsigned __int128)1 << (K - 1);

    unsigned __int128 sum_scaled = 0;
    idx_t limit = (n - 1) / 2;

    for (idx_t i = 1; i <= limit; ++i) {
        unsigned __int128 ii = i;
        unsigned __int128 jj = (unsigned __int128)(n - i);
        unsigned __int128 d2 = ii * ii + jj * jj;          // i^2 + (n-i)^2
        unsigned __int128 value = (d2 * 4) << SHIFT;      // (4*d2) * 2^(2K)
        seq_t term_scaled = sqrt128(value);               // floor(2*sqrt(d2) * 2^K)
        sum_scaled += (unsigned __int128)term_scaled;
    }

    unsigned __int128 rounded = (sum_scaled + ROUND_ADJ) >> K;  // round to nearest
    return (seq_t)rounded;
}
