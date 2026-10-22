/*
 * A336649  Sum of divisors of A336651(n) (odd part of n divided by its
 *          largest squarefree divisor).
 * https://oeis.org/A336649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 728, u16 n <= 9529, u32 n <= 9529, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative formula:
   a(2^e) = 1
   a(p^1) = 1
   a(p^e) = (p^e-1)/(p-1) = 1 + p + p^2 + … + p^(e-1)   for odd primes p and e > 1
   Thus a(n) is the product of these sums over all odd prime powers of n,
   ignoring factors 2 or prime powers of exponent 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t temp = n;
    // Remove all factors of 2 – they contribute multiplicatively 1
    while ((temp & 1) == 0)   // temp % 2 == 0
        temp >>= 1;           // temp /= 2

    seq_t result = 1;

    /* Factor remaining odd part by trial division. */
    for (idx_t p = 3; (p * p) <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp > 1) {
                /* Compute sum 1 + p + p^2 + … + p^(exp-1) */
                seq_t p_pow = 1;   // p^0
                seq_t sum = 0;
                for (idx_t i = 0; i < exp; ++i) {
                    sum += p_pow;
                    p_pow *= p;
                }
                result *= sum;
            }
        }
    }

    /* If any factor remains, it is a prime > sqrt(n) with exponent 1,
       and contributes multiplicatively 1 – so we ignore it. */

    return result;
}
