/*
 * A334933  a(n) = Product_{p|n, p<n, p prime} omega(n/p).
 * https://oeis.org/A334933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p|n, p<n, p prime} ω(n/p)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* arrays large enough for all uint64_t factorizations */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    if (cnt == 0)
        return 1;

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* skip the prime divisor equal to n itself (i.e. when n is prime) */
        if (primes[i] == (uint64_t)n)
            continue;
        /* omega(n/p) = cnt if exponent >1, else cnt-1 */
        seq_t term = (exps[i] > 1) ? (seq_t)cnt : (seq_t)(cnt - 1);
        result *= term;
    }
    return result;
}
