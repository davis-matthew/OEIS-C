/*
 * A262208  a(n) = prime(n)^prime(n) mod n^n.
 * https://oeis.org/A262208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^prime(n) mod n^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, mod_pow, pow_mod2pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* get_prime(idx_t k) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/mod_pow.h"    /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/pow_mod2pow.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime. */
    uint32_t p32 = get_prime(n);
    seq_t p   = (seq_t)p32;

    /* Compute the modulus m = n^n.  For 64‑bit builds this may overflow
       to zero; we test for that and use a special routine. */
    seq_t mod = seq_pow((seq_t)n, n);

    if (mod == 0) {
        /* Overflow: the actual modulus is 2^64, so perform exponentiation
           modulo 2^64, which is equivalent to keeping only the low 64 bits. */
        return pow_mod2pow(p, p);
    } else {
        /* Standard modular exponentiation with the exact modulus. */
        return mod_pow(p, (uint64_t)p32, mod);
    }
}
