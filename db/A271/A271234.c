/*
 * A271234  2^(p-1) modulo p^3, where p = prime(n).
 * https://oeis.org/A271234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(prime(n)-1) mod prime(n)^3
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* Get the n-th prime (1-indexed). */
    uint32_t p = get_prime(n);
    /* Compute modulus p^3. */
    seq_t mod = pow_int((seq_t)p, 3);
    /* Compute 2^(p-1) mod p^3 but base is p itself: 2^(p-1) mod p^3 */
    /* The sequence definition uses base 2, not p. Use mod_pow with base 2. */
    seq_t base2 = 2;
    seq_t result = mod_pow(base2, (uint64_t)(p - 1), mod);
    return result;
}
