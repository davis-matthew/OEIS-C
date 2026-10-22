/*
 * A325837  The number of coreful divisors of n that are exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A325837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} ⌊(e+1)/2⌋ (multiplicative, a(p^e)=⌊(e+1)/2⌋)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* seq_t a(idx_t n)
 * Computes the number of coreful divisors of n that are exponentially odd numbers.
 * It uses the multiplicative definition a(p^e) = ⌊(e+1)/2⌋.
 */
seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS definition; for n == 1 the loop below does nothing and
       result stays 1, which is the correct value. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 is provided by the build; it fills primes[], exps[] and cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t val = (seq_t)((exps[i] + 1) / 2);   /* floor((e+1)/2) */
        result *= val;
    }
    return result;
}
