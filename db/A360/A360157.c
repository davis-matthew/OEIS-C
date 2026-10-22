/*
 * A360157  a(n) is the number of unitary divisors of n that are odd squares.
 * https://oeis.org/A360157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over odd primes p|n of (exponent of p in n even ?
 *            2 : 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) counts odd square unitary divisors of n. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* special case n == 1 */
    if (n == 1) return (seq_t)1;

    /* factor_u64 will fill primes[], exps[] and cnt with the prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count odd primes whose exponent is even */
    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) continue;          /* ignore factor 2 (odd divisor must be odd) */
        if ((exps[i] & 1ULL) == 0) ++k;        /* exponent even -> contributes a factor of 2 */
    }

    /* compute 2^k */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < k; ++i)
        result <<= 1;       /* multiply by 2 */

    return result;
}
