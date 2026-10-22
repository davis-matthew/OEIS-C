/*
 * A335852  Product of the exponents in the prime factorization of n in the
 *          ring of Gaussian integers.
 * https://oeis.org/A335852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) with f(p,e) = 2·e if p = 2 e if p ≡ 3
 *            (mod 4) e² if p ≡ 1 (mod 4) (multiplicative) a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;          /* work in uint64_t for factorisation */
    seq_t result   = (seq_t)1;
    uint64_t e, p;

    /* handle prime 2 separately */
    e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e++;
    }
    if (e > 0)
        result *= (seq_t)(2 * e);

    /* handle odd primes up to sqrt(temp) */
    for (p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (p % 4 == 3)          /* p ≡ 3 (mod 4) */
                result *= (seq_t)(e);
            else                     /* p ≡ 1 (mod 4) */
                result *= (seq_t)(e * e);
        }
    }

    /* any remaining factor > sqrt(original n) is a prime with exponent 1 */
    /* its contribution is 1 for both mod 4 cases, so we can ignore */

    return result;
}
