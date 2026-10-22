/*
 * A366742  The number of divisors of the least coreful infinitary divisor of
 *          n.
 * https://oeis.org/A366742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over each prime factor p^e of n of (2^{v₂(e)} +
 *            1) * where v₂(e) is the exponent of 2 in the factorisation of e
 *            (i.e., * the number of trailing zero bits in e). This follows
 *            from the fact that * the least coreful infinitary divisor of n
 *            is * LCF(n) = ∏ p^{2^{v₂(e)}}, * and the numb...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* safety for indices < 1 */

    uint64_t N = (uint64_t)n;
    /* Allocate space for prime factors and exponents (max 64 factors is safe). */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t count = 0;

    /* Factorise N.  The function writes the number of prime factors into count. */
    factor_u64(N, primes, exponents, &count);

    seq_t result = 1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t e = exponents[i];
        /* number of trailing zero bits in e: v₂(e) */
        unsigned k = __builtin_ctzll(e);        /* e > 0, so defined */
        seq_t exp_lcf = ((seq_t)1) << k;        /* 2^{v₂(e)} */
        result *= (exp_lcf + (seq_t)1);          /* multiply by exponent+1 */
    }

    return result;
}
