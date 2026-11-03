/*
 * A369747  a(n) = Sum_{p|n, p prime} pi(n/p).
 * https://oeis.org/A369747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} pi(n/p)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* n == 1 gives 0, n == 0 is not used but safe */
        return (seq_t)0;

    /* Factor n into primes.  64 slots are plenty for any 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        idx_t divisor = (idx_t)primes[i];
        /* pi(n/divisor) */
        sum += prime_pi(n / divisor);
    }
    return sum;
}
