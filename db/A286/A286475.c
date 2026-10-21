/*
 * A286475  Compound filter (for counting primes of form 6k+1, 6k+2, 6k+3 and
 *          6k+5): a(n) = 6*A032742(n) + (A020639(n) mod 6), a(1) = 1.
 * https://oeis.org/A286475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(n / spf(n)) + (spf(n) mod 6), with a(1)=1 where spf is
 *            the smallest prime factor of n
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t uf = (uint64_t)n;
    uint64_t sp = _smallest_prime_factor(uf);
    seq_t q   = (seq_t)(uf / sp);
    seq_t mod = (seq_t)(sp % 6);

    return q * 6 + mod;
}
