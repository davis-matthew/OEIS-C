/*
 * A265012  a(n) = 10^(prime(n)-1) mod prime(n)^2.
 * https://oeis.org/A265012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^(prime(n)-1) mod prime(n)^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6566, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute the modulus p^2. */
    seq_t mod = (seq_t)p * p;

    /* Compute 10^(p‑1) mod p^2.  mod_pow handles arbitrary‑precision
       types as needed. */
    return mod_pow((seq_t)10, (uint64_t)(p - 1), mod);
}
