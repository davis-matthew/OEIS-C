/*
 * A369253  Parity of A038548, where A038548(n) is the number of divisors of n
 *          that are at most sqrt(n).
 * https://oeis.org/A369253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = parity of A038548(n) = ((tau(n)+is_square(n))/2) mod 2
 * Range:     u8 n <= 7435, u16 n <= 7435, u32 n <= 7435, u64 n <= 7435,
 *            u128 n <= 7435, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for uint64_t */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);      /* hi^2 > max 64‑bit value */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;        /* upper mid */
        if (mid <= n / mid) /* mid*mid <= n without overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)             /* undefined for n=0, return 0 */
        return (seq_t)0;

    idx_t nn = n;           /* copy for factorization */
    uint8_t tau_mod4 = 1;   /* τ(n) modulo 4, accumulate as (a*b)&3 */

    /* trial division up to sqrt(nn); recompute limit as factor found */
    idx_t limit = isqrt_u64(nn);
    for (idx_t p = 2; p <= limit && nn > 1; ++p)
    {
        if (nn % p != 0)
            continue;

        /* count exponent of p in nn */
        idx_t exp = 0;
        do {
            nn /= p;
            ++exp;
        } while (nn % p == 0);

        tau_mod4 = (uint8_t)((tau_mod4 * ((exp + 1) & 3)) & 3);
        limit = isqrt_u64(nn);
    }
    if (nn > 1)          /* leftover prime factor */
        tau_mod4 = (uint8_t)((tau_mod4 * 2) & 3);

    /* check if n is a perfect square */
    idx_t sqroot = isqrt_u64(n);
    uint8_t is_square = (sqroot * sqroot == n) ? 1 : 0;

    /* parity: 1 if (tau_mod4 + is_square) mod 4 == 2, else 0 */
    uint8_t sum_mod4 = (uint8_t)((tau_mod4 + is_square) & 3);
    return (seq_t)((sum_mod4 == 2) ? 1 : 0);
}
