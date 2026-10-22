/*
 * A377850  Noll index series of Zernike polynomials converted to Fringe
 *          index.
 * https://oeis.org/A377850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(j) = (1 + (n + |m|)/2)^2 - 2*|m| + [m < 0] where n =
 *            floor((sqrt(8*(j-1)+1) - 1)/2) m = (-1)^j * (n mod 2 +
 *            2*floor((j - n*(n+1)/2 - 1 + (n+1) mod 2)/2))
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 32;       /* enough for all disc values */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* guard for invalid offset */

    unsigned __int128 disc = (unsigned __int128)8 * ( (unsigned __int128)n - 1 ) + 1;
    uint64_t sqrt_disc = isqrt_uint128(disc);
    uint64_t nn = (sqrt_disc - 1) / 2;                    /* n in the formula */

    uint64_t t_tri = nn * (nn + 1) / 2;                   /* triangular number T_n */
    uint64_t k = n - t_tri - 1;                           /* j - T_n - 1 */

    uint64_t mod_n2   = nn & 1;
    uint64_t mod_np12 = (nn + 1) & 1;
    uint64_t inner    = (k + mod_np12) / 2;               /* floor((k + mod(n+1,2))/2) */

    uint64_t abs_m = mod_n2 + 2 * inner;                 /* |m| part */
    int sign = (n & 1) ? -1 : +1;                         /* (-1)^j, 1-indexed */
    int m = sign * (int)abs_m;

    seq_t abs_m_s = (seq_t)abs_m;
    seq_t t1 = ((seq_t)nn + abs_m_s) / (seq_t)2;
    seq_t t  = (seq_t)1 + t1;
    seq_t result = t * t - (seq_t)2 * abs_m_s;
    if (m < 0)
        result += 1;
    return result;
}
