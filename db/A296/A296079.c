/*
 * A296079  a(n) = 1 if 1+phi(n) is prime, 0 otherwise, where phi = A000010,
 *          Euler totient function.
 * https://oeis.org/A296079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 1+phi(n) is prime, 0 otherwise
 * Range:     u8 n <= 4039, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   is_prime_big, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);        /* Euler's totient φ(n) */
    seq_t test = phi_val + (seq_t)1; /* 1 + φ(n) */
    return is_prime_big(test) ? (seq_t)1 : (seq_t)0;
}
