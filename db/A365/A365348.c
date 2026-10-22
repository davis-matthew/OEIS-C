/*
 * A365348  The number of divisors of the smallest exponentially odd number
 *          divisible by n.
 * https://oeis.org/A365348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A000005(A356191(n)) = product over prime exponents of
 *            (a_i + 2 - (a_i mod 2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n, adjusts each exponent to the nearest odd value >= it,
   and returns the number of divisors of the resulting exponentially odd number. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;      /* undefined for 0, but guard just in case */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];              /* room for all prime factors of a 64‑bit integer */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t a = exps[i];                 /* exponent of prime i in n */
        seq_t contrib = (seq_t)(a + 2 - (a & 1U)); /* a + 2 - (a mod 2) */
        result *= contrib;
    }
    return result;
}
