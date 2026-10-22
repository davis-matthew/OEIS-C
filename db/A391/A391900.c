/*
 * A391900  The total number of prime factors (counted with multiplicity) of
 *          the prime factorization exponents of n.
 * https://oeis.org/A391900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e||n} bigomega(e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n < 1 */
    seq_t sum = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);
    for (size_t i = 0; i < count; ++i) {
        sum += (seq_t)bigomega((idx_t)exps[i]);
    }
    return sum;
}
