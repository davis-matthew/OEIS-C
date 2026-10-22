/*
 * A329838  Beatty sequence for (6+sqrt(26))/5.
 * https://oeis.org/A329838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (6 + sqrt(26)) / 5)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula: a(n) = floor( (6*n + floor(n*sqrt(26))) / 5 ) */

#include <stdint.h>

/* Compute the integer square root of an unsigned __int128 value.
   Returns the greatest t with t*t <= x. */
static uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* hi < lo, and hi^2 <= x < (hi+1)^2 */
}

/* Return the n-th term of OEIS A329838. */
seq_t a(idx_t n)
{
    /* n is 1-based.  No special handling for n == 0 needed. */
    /* Compute floor(n*sqrt(26)).  Use integer arithmetic. */
    unsigned __int128 nsq = (unsigned __int128)n * (unsigned __int128)n;
    unsigned __int128 prod = (unsigned __int128)26 * nsq;
    uint64_t k = isqrt_u128(prod);          /* floor(n*sqrt(26)) */

    /* Compute floor((6*n + k) / 5) using 128-bit arithmetic to avoid overflow. */
    unsigned __int128 temp = (unsigned __int128)6 * n + k;
    seq_t res = (seq_t)(temp / 5);
    return res;
}
