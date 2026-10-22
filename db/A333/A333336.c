/*
 * A333336  a(n) is the smallest positive number k such that n divides 5^k +
 *          k.
 * https://oeis.org/A333336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k > 0 such that n divides 5^k + k
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;                 /* safety, n should be ≥ 1 */

    uint64_t mod = n;                             /* modulus, fits in 64‑bit */
    uint64_t pow5 = 5 % mod;                      /* 5^1 mod n */

    for (idx_t k = 1; ; ++k)                      /* search for the minimal k */
    {
        /* (5^k + k) mod n == 0 ? */
        if ( ( ( (__uint128_t)pow5 + (uint64_t)k ) % mod ) == 0ULL )
            return (seq_t)k;

        /* next power of 5 modulo n */
        pow5 = (uint64_t)( ( (__uint128_t)pow5 * 5 ) % mod );
    }
}
