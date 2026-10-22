/*
 * A386472  The maximum exponent in the prime factorization of the largest
 *          divisor of n whose exponents in its prime factorization are
 *          squares.
 * https://oeis.org/A386472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, small_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A386472
   Formula: a(n) = max square ≤ exponent in a prime factorization of the
   largest divisor of n whose exponents are all squares.
   Simplification:  For n = ∏ pᵉ, a(n) = max{ (⌊√e⌋)² } with a(1)=0.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/small_isqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)                /* n==1 gives a(1)=0 */
        return 0;

    /* factor n into primes and exponents */
    uint64_t primes[64];   /* enough for any uint64_n */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];           /* exponent of prime i */
        idx_t root = small_isqrt(e);        /* ⌊√e⌋ */
        seq_t sq = (seq_t)root * (seq_t)root;/* (⌊√e⌋)² */
        if (sq > best)
            best = sq;
    }
    return best;
}
