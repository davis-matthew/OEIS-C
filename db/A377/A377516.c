/*
 * A377516  The number of divisors of n that are terms in A276078.
 * https://oeis.org/A377516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e || n} (min(π(p), e) + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where π(p) is the prime counting function A000720(p) */

#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Temporary storage for prime factors and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t pi = compute_pi((idx_t)primes[i]);          /* π(p) */
        seq_t e  = (seq_t)exps[i];                       /* exponent of p */
        seq_t val = (pi < e ? pi : e) + (seq_t)1;         /* min(π(p), e)+1 */
        result *= val;
    }

    return result;
}
