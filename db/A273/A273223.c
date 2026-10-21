/*
 * A273223  a(n) = p*(p - 1)*(501*p^3 - 414*p^2 + 111*p - 54)/120, where p =
 *          prime(n).
 * https://oeis.org/A273223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p-1)*(501*p^3 - 414*p^2 + 111*p - 54)/120, where p =
 *            prime(n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 18, u64 n <= 708,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime (p) */
    uint32_t p32 = get_prime(n);
    unsigned __int128 p = p32;

    /* compute the numerator term in 128-bit to avoid overflow */
    unsigned __int128 term = ((unsigned __int128)501 * p * p * p)
                           - ((unsigned __int128)414 * p * p)
                           + ((unsigned __int128)111 * p)
                           - (unsigned __int128)54;

    /* multiply by p*(p-1) and divide by 120, still in 128-bit */
    unsigned __int128 prod = p * (p - 1) * term;
    prod /= (unsigned __int128)120;

    return (seq_t)prod;
}
