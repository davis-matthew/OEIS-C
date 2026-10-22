/*
 * A360794  Expansion of Sum_{k>0} x^k / (1 - k * x^k)^(k+1).
 * https://oeis.org/A360794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d-1) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, value for 0 is unused */

    /* Factorise n */
    uint64_t primes[32], exps[32];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate all divisors of n */
    idx_t divis[256];
    size_t dcnt = 1;
    divis[0] = 1;

    for (size_t i = 0; i < pcnt; i++) {
        idx_t p = (idx_t)primes[i];
        int   e = (int)exps[i];
        size_t old = dcnt;

        idx_t mult = 1;
        for (int k = 1; k <= e; k++) {
            mult *= p;                     /* p^k */
            for (size_t j = 0; j < old; j++) {
                divis[dcnt++] = divis[j] * mult;
            }
        }
    }

    /* Compute the sum over divisors */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; i++) {
        idx_t d    = divis[i];
        idx_t exp  = n / d - 1;            /* exponent (n/d - 1) */
        seq_t pow  = big_pow((seq_t)d, (seq_t)exp);
        seq_t bin  = binomial(d + exp, d); /* binomial(d + n/d - 1, d) */
        sum += pow * bin;
    }
    return sum;
}
