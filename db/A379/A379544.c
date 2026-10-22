/*
 * A379544  a(n) = ((p-1)^n + (p+1)^n) mod p^2, where p is the n-th prime.
 * https://oeis.org/A379544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p-1)^n + (p+1)^n) mod p^2, where p is the n‑th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    /* Retrieve the n‑th prime (32‑bit) and cast it to seq_t */
    seq_t p = (seq_t)get_prime(n);

    /* Compute modulus p^2 */
    seq_t mod = p * p;

    /* Compute (p-1)^n mod p^2 and (p+1)^n mod p^2 */
    seq_t term1 = mod_pow(p - 1, (uint64_t)n, mod);
    seq_t term2 = mod_pow(p + 1, (uint64_t)n, mod);

    /* Sum and reduce modulo p^2 */
    seq_t res = term1 + term2;
    res %= mod;

    return res;
}
