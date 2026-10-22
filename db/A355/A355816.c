/*
 * A355816  a(n) = 1 if n and sigma(n) are relatively prime, and n has at
 *          least two distinct prime factors, otherwise 0.
 * https://oeis.org/A355816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has at least two distinct prime factors and
 *            gcd(n,σ(n)) = 1; otherwise 0
 * Range:     u8 n <= 2048, u16 n <= 9296, u32 n <= 9296, u64 n <= 9296,
 *            u128 n <= 9296, big any n
 * Helpers:   count_distinct_primes, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/count_distinct_primes.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                         /* undefined for n < 1, defined as 0 */

    /* n must have at least two distinct prime factors */
    if (count_distinct_primes((uint64_t)n) < 2)
        return 0;

    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* n and σ(n) must be coprime */
    return (gcd((seq_t)n, sigma) == 1) ? 1 : 0;
}
