/*
 * A376334  Integer part of the product of three consecutive primes divided by
 *          their sum.
 * https://oeis.org/A376334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( p_n * p_{n+1} * p_{n+2} / (p_n + p_{n+1} +
 *            p_{n+2}) )
 * Range:     u8 n <= 9, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* retrieve the three consecutive primes */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);

    /* use 128‑bit arithmetic to avoid overflow during the multiplication */
    __int128 product = (__int128)p1 * p2 * p3;
    __int128 sum     = (__int128)p1 + p2 + p3;

    /* integer division yields the floor of the fraction */
    seq_t result = (seq_t)(product / sum);
    return result;
}
