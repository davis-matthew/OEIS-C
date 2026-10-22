/*
 * A337363  a(n) = Sum_{d1|n, d2|n, d1<d2} (1 - [d1 + 1 = d2]), where [ ] is
 *          the Iverson bracket.
 * https://oeis.org/A337363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total pairs of divisors of n minus the pairs of
 *            consecutive divisors, i.e. a(n) = C(tau(n),2) - |{ d | d|n and
 *            d+1|n }|
 * Range:     u8 n <= 359, u16 n <= 9778, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   count_consecutive, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/count_consecutive.h"

seq_t a(idx_t n)
{
    /* factor the integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* number of divisors tau(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* total unordered pairs of divisors */
    seq_t totalPairs = tau * (tau - 1) / 2;

    /* count consecutive pairs via recursion */
    uint64_t consecutive = 0;
    count_consecutive(0, 1, primes, exps, cnt, (uint64_t)n,
                      &consecutive);

    return totalPairs - (seq_t)consecutive;
}
