/*
 * A365349  The sum of divisors of the smallest exponentially odd number
 *          divisible by n.
 * https://oeis.org/A365349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A365349
   a(n) = sigma( n * Π_{p|n , v_p(n) even} p )
   which equals
     ∏_{p^e∥n} ( p^{e+2-(e%2)} – 1 ) / (p – 1)
   σ is the divisor sum function.
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 0) return 0;   /* offset is 1; n≥1 in actual usage */

    /* factor n. 64‑bit integers can have at most 64 prime factors (including repetitions). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* exponent in the smallest exponentially odd multiple of n   */
        uint64_t f = (e & 1ULL) ? e : e + 1ULL;

        /* sigma(p^f) = (p^(f+1) – 1) / (p – 1) */
        seq_t p_seq = (seq_t)p;
        seq_t numerator   = big_pow(p_seq, (seq_t)(f + 1ULL)) - (seq_t)1;
        seq_t denominator = p_seq - (seq_t)1;
        seq_t sigma = numerator / denominator;

        result *= sigma;
    }

    return result;
}
