/*
 * A336695  a(n) = A331410(1+sigma(n)), where A331410 is totally additive with
 *          a(2) = 0 and a(p) = 1 + a(p+1) for odd primes.
 * https://oeis.org/A336695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410(1+σ(n)) where * A331410 is totally additive,
 *            A331410(2)=0, and * A331410(p)=1+A331410(p+1) for odd primes p.
 *            (σ(n)=sigma(n))
 * Range:     u8 n <= 2145, u16 n <= 9963, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

/* Recursive evaluation of A331410.
 *  f_value(n) computes A331410(n) for any integer n>=1.
 *  f_prime(p) computes A331410(p) for an odd prime p.
 *  Both use the definition that A331410 is totally additive and
 *  that A331410(2)=0.
 */
static seq_t f_prime(uint64_t p);

static seq_t f_value(seq_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;
    uint64_t temp   = (uint64_t)n;

    /* Handle the factor 2 (contributes 0 to the sum). */
    while ((temp & 1) == 0)
        temp >>= 1;          /* divide by 2 */

    /* Factor remaining odd part. */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            result += (seq_t)exp * f_prime(p);
        }
    }

    /* If a prime factor > sqrt(temp) remains. */
    if (temp > 1)
        result += f_prime(temp);

    return result;
}

static seq_t f_prime(uint64_t p)
{
    /* For p==2, the value is defined to be 0. */
    if (p == 2)
        return 0;

    /* Recursive definition:  f(p) = 1 + f((p+1)/2). */
    return 1 + f_value((seq_t)((p + 1) / 2));
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* A336695 is defined for n >= 1 (offset 1). */
    /* For n < 1 we return 0 but the build never calls this. */
    if (n < 1)
        return 0;

    /* Compute 1 + σ(n).  sigma_of returns seq_t; cast to uint64_t
     * because the values for n <= 10 000 are well below 2^64. */
    uint64_t sigma = (uint64_t)sigma_of(n);
    uint64_t m     = sigma + 1u;

    return f_value((seq_t)m);
}
