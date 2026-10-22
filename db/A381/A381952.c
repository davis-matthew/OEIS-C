/*
 * A381952  a(n) is the greatest common divisor of n and the maximum exponent
 *          in the prime factorization of n.
 * https://oeis.org/A381952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, A051903(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051903(n) is the maximum exponent in the prime factorization of n. */
/* The implementation uses the provided factor_u64 utility to obtain the
 * prime exponents, then computes the greatest common divisor of n and
 * the largest exponent. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 (outside the official range).  Return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Factor n and find the maximal exponent. */
    /* 64 is safely larger than the maximum number of distinct prime
     * factors of any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    factor_u64((uint64_t)n, primes, exps, &count);

    uint64_t max_exp = 0;
    for (size_t i = 0; i < count; ++i)
        if (exps[i] > max_exp)
            max_exp = exps[i];

    /* gcd of seq_t arguments. */
    return gcd((seq_t)n, (seq_t)max_exp);
}
