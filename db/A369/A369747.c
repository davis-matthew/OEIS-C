/*
 * A369747  a(n) = Sum_{p|n, p prime} pi(n/p).
 * https://oeis.org/A369747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} pi(n/p)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

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
        sum += compute_pi(n / divisor);
    }
    return sum;
}
