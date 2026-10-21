/*
 * A283419  a(n) is the multiplicative inverse of 3 modulo the n-th prime (or
 *          0 if none exists).
 * https://oeis.org/A283419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the multiplicative inverse of 3 modulo the nth prime (or
 *            0 if that prime is 3). The inverse is computed by modular
 *            exponentiation a(n) = 3^(p-2) mod p for p = prime(n), p ≠ 3. For
 *            p = 3 the function returns 0
 * Range:     u8 n <= 54, u16 n <= 6691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Index 1 corresponds to the first prime (2).  Behaviour for n < 1 is
       unimportant; the sequence starts at offset 1. */
    uint32_t p = get_prime(n);

    /* The prime 3 has no multiplicative inverse modulo itself. */
    if (p == 3)
        return 0;

    /* For a prime p > 3, Euler's theorem gives 3^(p-2) ≡ 3^(-1) (mod p). */
    uint64_t exp = (uint64_t)p - 2;
    return mod_pow((seq_t)3, exp, (seq_t)p);
}
