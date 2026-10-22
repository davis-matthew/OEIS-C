/*
 * A358345  a(n) is the number of even square divisors of n.
 * https://oeis.org/A358345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(v₂(n)/2) * ∏_{p odd prime|n} (floor(v_p(n)/2)+1)
 *            where v_p(n) is the exponent of prime p in n. This yields the
 *            number of even square divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"    /* provides factor_u64 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;        /* n=1 ⇒ 0 even square divisors */

    /* Pull apart n into its prime factors */
    uint64_t factors[64], exponents[64];
    size_t count = 0;
    factor_u64((uint64_t)n, factors, exponents, &count);

    seq_t odd_part = 1;          /* product of (floor(e/2)+1) for odd primes */
    seq_t two_choices = 0;       /* floor(e₂/2), number of even square factors along 2‑axis */

    for (size_t i = 0; i < count; ++i)
    {
        uint64_t prime  = factors[i];
        uint64_t exp    = exponents[i];

        if (prime == 2ULL)
        {
            two_choices = exp / 2;    /* floor(e₂/2) */
        }
        else
        {
            odd_part *= (seq_t)(exp / 2 + 1ULL);   /* floor(e/2)+1 */
        }
    }

    if (two_choices == 0) return (seq_t)0;   /* no even square divisor possible */

    return two_choices * odd_part;
}
