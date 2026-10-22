/*
 * A367512  Number of evil exponents in the prime factorization of n.
 * https://oeis.org/A367512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime exponents e of n that are evil (i.e., e
 *            has an even number of 1‑bits in binary)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_evil_exponent, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/factor_u64.h"    /* external function: factor_u64 */
#include "../utils/_is_evil_exponent.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety: we care about n ≥ 1 only */

    /* Factorization of n; we only need the exponents. */
    uint64_t primes[64];            /* enough for any 64‑bit n */
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += _is_evil_exponent(exps[i]);
    }
    return sum;
}
