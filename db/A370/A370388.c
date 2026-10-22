/*
 * A370388  First appearance of prime_i in A369797 or 0 if no such appearance
 *          occurs, with p_0 being 1. (A008578 but with a different offset.)
 * https://oeis.org/A370388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 * inv3) mod prime(n) for n > 2; * where inv3 is the
 *            modular inverse of 3 modulo prime(n). * Special cases: a(0) =
 *            10, a(1) = 6, a(2) = 0 * (the convention for p₀ is 1; primes for
 *            n ≥ 1 are standard primes)
 * Range:     u8 n <= 54, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the few exceptional cases */
    if (n == 0)
        return (seq_t)10;
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)0;

    /* For n >= 3 we use the formula */
    uint64_t p = get_prime(n);          /* prime(n)  (n ≥ 3 → odd prime > 3) */
    uint64_t inv3;                      /* modular inverse of 3 mod p */

    /* Since p ≡ 1 or 2 (mod 3), the inverse is obtained without
       extended Euclid:
       - if p % 3 == 1:  inv3 = (2*p + 1) / 3
       - if p % 3 == 2:  inv3 = (p + 1) / 3
    */
    if (p % 3 == 1)
        inv3 = (2 * p + 1) / 3;
    else
        inv3 = (p + 1) / 3;

    /* Compute 2 * inv3 modulo p */
    seq_t val = ((seq_t)2 * (seq_t)inv3) % (seq_t)p;
    return val;
}
