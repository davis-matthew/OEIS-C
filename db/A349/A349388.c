/*
 * A349388  Dirichlet convolution of A000027 with A346234 (Dirichlet inverse
 *          of A003961), where A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A349388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 242, i16 n <= 8913, i32 n <= 8913, i64 n <= 8913,
 *            i128 n <= 8913, big any n
 * Helpers:   factor_u64, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Multiplicative formula for OEIS A349388
   a(n) = product_{p^e || n} (p^e - nextprime(p)*p^(e-1))
   with nextprime(p) the smallest prime > p.
   For n = 1 the empty product equals 1.
   The implementation factors n using factor_u64, then
   multiplies the contributions of each prime power.
*/
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but guard to be safe */

    /* factorization buffers – max 64 prime factors is plenty for 64-bit n */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* common factor p^(e-1) */
        seq_t common = seq_pow((seq_t)p, e - 1);

        /* difference p - nextprime(p) */
        seq_t diff = (seq_t)p - (seq_t)next_prime(p);

        /* term = common * diff */
        seq_t term = common * diff;

        result *= term;
    }

    return result;
}
