/*
 * A250096  Numerator of the harmonic mean of the prime factors, without
 *          multiplicity, of n.
 * https://oeis.org/A250096
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the numerator of the harmonic mean of the distinct prime
 *            factors of n
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* we use factor_u64 to obtain the distinct prime factors,
   compute P (product), S (sum of P/pi), reduce the fraction
   k*P / S, and return the numerator */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2u)   /* below the offset – not used by tests */
        return 0;

    /* factor_u64 returns primes in the array 'pr', exponents in 'ex',
       and the number of distinct primes in '*cnt'                */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    if (cnt == 0)   /* n == 1 */
        return 0;

    /* product of distinct primes */
    seq_t P = 1;
    for (size_t i = 0; i < cnt; ++i)
        P *= (seq_t)pr[i];

    /* sum of P / pi */
    seq_t S = 0;
    for (size_t i = 0; i < cnt; ++i)
        S += P / (seq_t)pr[i];

    /* numerator before reduction */
    seq_t num = (seq_t)cnt * P;

    /* reduce the fraction num/S */
    seq_t g = gcd(num, S);
    return num / g;
}
