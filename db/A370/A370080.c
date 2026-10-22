/*
 * A370080  The product of the even exponents of the prime factorization of n.
 * https://oeis.org/A370080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of all even exponents in the prime factorization
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: if n = ∏ p_i^e_i then a(n) = ∏_i (e_i  if e_i even, otherwise 1) */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for OEIS sequence A370080. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1, and n < 1 is never requested */

    seq_t result = (seq_t)1;
    idx_t temp   = n;
    idx_t exp    = 0;

    /* Factor 2 separately. */
    while ((temp & 1ULL) == 0ULL) {
        ++exp;
        temp >>= 1;
    }
    if (exp % 2 == 0 && exp > 0)
        result *= (seq_t)exp;

    /* Factor odd primes. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            if (exp % 2 == 0)
                result *= (seq_t)exp;
        }
    }

    /* If temp > 1 now, it is a remaining prime factor with exponent 1
       (odd), so it contributes nothing to the product. */

    return result;
}
