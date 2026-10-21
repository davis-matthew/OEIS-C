/*
 * A286283  a(n) = floor(7*n^2/48).
 * https://oeis.org/A286283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*n^2/48)
 * Range:     u8 n <= 41, u16 n <= 670, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n*n in an intermediate type large enough
       to hold the product without overflow. */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 uint128_t;
    uint128_t nn = (uint128_t)(seq_t)n;          /* cast idx_t to seq_t then to uint128_t */
#else
    /* Fallback: use 64‑bit arithmetic; for n up to about 4·10^4
       this is sufficient for usual test ranges. */
    typedef unsigned long long uint128_t;
    uint128_t nn = (uint128_t)(seq_t)n;
#endif

    uint128_t prod = nn * nn * 7ULL;            /* 7*n^2  */
    uint128_t result = prod / 48ULL;            /* floor division */

    return (seq_t)result;
}
