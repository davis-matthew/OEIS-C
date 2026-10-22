/*
 * A360161  a(n) is the sum of unitary divisors of n that are odd squares
 *          minus the sum of unitary divisors of n that are even squares.
 * https://oeis.org/A360161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1, d square} (-1)^(d+1) * d
 * Range:     i8 n <= 143, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative with the rules
   a(2^e) = { 1-2^e  if e is even
           { 1      if e is odd
   a(p^e) = { p^e+1  if e is even
           { 1      if e is odd
   for p odd. */

/* The implementation below uses the supplied factor_u64 routine to
   factor n.  For each prime power we compute the local contribution
   as described above and multiply them together. */

/* An O(log n) implementation that works for all supported builds
   (int8_t ... __int128 ... big integer) as long as the intermediate
   powers fit in uint64_t – which they do for all n ≤ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;   /* a(1) = 1 */

    /* At most 16 distinct prime factors for a 64‑bit integer. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib = (seq_t)1;

        if (p == 2ULL) {
            if ((e & 1ULL) == 0ULL) {   /* e even */
                uint64_t pow2 = 1ULL;
                for (uint64_t k = 0; k < e; ++k) pow2 *= 2ULL;
                contrib = (seq_t)( (int64_t)1 - (int64_t)pow2 );
            }
            /* e odd: contrib stays 1 */
        } else { /* odd prime */
            if ((e & 1ULL) == 0ULL) {   /* e even */
                uint64_t powp = 1ULL;
                for (uint64_t k = 0; k < e; ++k) powp *= p;
                contrib = (seq_t)( (int64_t)powp + 1 );
            }
            /* e odd: contrib stays 1 */
        }

        result *= contrib;
    }

    return result;
}
