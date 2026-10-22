/*
 * A392629  The number of coreful infinitary divisors of n that are numbers
 *          whose number of divisors is a power of 2.
 * https://oeis.org/A392629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^e||n} t_ones(e) where * t_ones(e) = number of
 *            consecutive 1‑bits in e starting * from the least significant
 *            bit. * This implements the multiplicative definition from OEIS
 *            A392629
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* count trailing 1-bits in exponent e */
static inline uint64_t trailing_ones(uint64_t e)
{
    uint64_t cnt = 0;
    while ((e & 1ULL) != 0) {
        ++cnt;
        e >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* n == 1 has no prime factors – multiplicative identity */
    if (n == (idx_t)1) return (seq_t)1;

    /* factorize n, exponent arrays up to 64 primes suffice for 64‑bit n */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        uint64_t t = trailing_ones(e);   /* number of coreful infinitary divisors with
                                            number of divisors a power of 2 */
        result *= t;                     /* seq_t multiplication works for all seq_t */
    }
    return result;
}
