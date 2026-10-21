/*
 * A297491  a(n) = (1/2) * Sum_{|k|<=2*sqrt(p)} k^4*H(4*p-k^2) where H() is
 *          the Hurwitz class number and p is n-th prime.
 * https://oeis.org/A297491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n)^3 - 3*prime(n) - 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 209, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p_u32 = get_prime(n);      /* nth prime (n ≥ 1) */
    seq_t p = (seq_t)p_u32;             /* cast to the build's seq_t type   */
    seq_t result = 2 * p * p * p - 3 * p - 1;
    return result;
}
