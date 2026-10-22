/*
 * A337362  Number of pairs of divisors of n, (d1,d2), with d1 <= d2 such that
 *          d1 and d2 are nonconsecutive integers.
 * https://oeis.org/A337362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n)(τ(n)+1)/2 - #{ d | n : d+1 | n }
 * Range:     u8 n <= 359, u16 n <= 7543, u32 n <= 7543, u64 n <= 7543,
 *            u128 n <= 7543, big any n
 * Helpers:   _count_consecutive, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_count_consecutive.h"

seq_t a(idx_t n)
{
    /* n is uint64_t for factorization */
    uint64_t n64 = (uint64_t)n;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(n64, primes, exps, &pf_cnt);

    /* compute τ(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        tau *= (seq_t)(exps[i] + 1ULL);
    }

    /* total unordered divisor pairs (incl. equal) */
    seq_t total = tau * (tau + 1) / 2;

    /* count consecutive divisor pairs */
    seq_t consec = 0;
    _count_consecutive(0, 1, n64, primes, exps, pf_cnt, &consec);

    return total - consec;
}
