/*
 * A316347  a(n) = n^2 mod(10^m), where m is the number of digits in n
 *          (written in base 10).
 * https://oeis.org/A316347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n*n mod 10^digit_count(n)
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"

/* Helper: compute 10^m as an unsigned __int128 */
static inline unsigned __int128 pow10_u128(idx_t m)
{
    unsigned __int128 r = 1;
    while (m--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* number of decimal digits of n */
    idx_t m = digit_count((uint64_t)n);

    /* modulus 10^m as a 128‑bit integer -- always fits */
    unsigned __int128 mod   = pow10_u128(m);

    /* reduce n modulo mod, then compute the square modulo mod */
    unsigned __int128 x     = (unsigned __int128)n % mod;
    unsigned __int128 y     = (x * x) % mod;

    /* cast the result to the output type */
    return (seq_t)y;
}
