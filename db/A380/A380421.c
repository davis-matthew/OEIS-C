/*
 * A380421  a(n) is the inverse of 2^3 modulo prime(n).
 * https://oeis.org/A380421
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 8^(-1) (mod prime(n)) for n >= 2
 * Range:     u8 n <= 54, u16 n <= 6654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (inverse of 8 modulo the nth prime) */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for indices below the offset */

    uint32_t p32 = get_prime(n);          /* nth prime (prime(1)=2) */
    seq_t p = (seq_t)p32;                 /* modulus */

    seq_t base = (seq_t)8;                /* number to invert */
    uint64_t exp = (uint64_t)p32 - 2ULL;   /* by Fermat: 8^(p-2) mod p */

    seq_t inv = mod_pow(base, exp, p);
    return inv;
}
