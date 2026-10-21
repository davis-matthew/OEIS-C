/*
 * A260531  a(n) = (2^p+1)^(p-1) modulo p^2, where p is prime(n).
 * https://oeis.org/A260531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^p + 1)^(p-1) mod p^2, where p = prime(n)
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 6560, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p = get_prime(n);

    /* modulus p^2 */
    seq_t mod = (seq_t)p * (seq_t)p;

    /* base = (2^p + 1) mod p^2 */
    seq_t base = (mod_pow((seq_t)2, (uint64_t)p, mod) + (seq_t)1) % mod;

    /* exponent p-1 */
    uint64_t exp = (uint64_t)p - 1;

    /* result = base^exp mod p^2 */
    return mod_pow(base, exp, mod);
}
