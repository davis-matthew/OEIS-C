/*
 * A256692  From fifth root of Riemann zeta function: form Dirichlet series
 *          Sum b(n)/n^x whose fifth power is zeta function; sequence gives
 *          numerator of b(n).
 * https://oeis.org/A256692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256692:  numerator of the Dirichlet coefficients b(n)
   of (ζ(s))^(1/5), i.e. the 5th Dirichlet root of the constant
   sequence 1.  The Dirichlet coefficient b(p^e) for a prime power
   equals \binom{1/5+e-1}{e} = \prod_{j=1}^e (5*j-4)/(5^e·e!).
   The overall b(n) is multiplicative and the numerator of the fully
   reduced fraction is returned.  */
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    /* maximum number of distinct prime factors for n≤10⁴ is <20,
       so 64 slots is safe */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64(nn, primes, exps, &pcnt);

    seq_t num = 1;   /* overall numerator */
    seq_t den = 1;   /* overall denominator */

    for (size_t i = 0; i < pcnt; ++i) {
        unsigned e = (unsigned)exps[i];   /* exponent of prime p_i */

        /* numerator of b(p^e) : prod_{j=1}^e (5*j-4) */
        seq_t num_p = 1;
        for (unsigned j = 1; j <= e; ++j) {
            seq_t factor = (seq_t)(5 * (uint64_t)j - 4);
            num_p *= factor;
        }

        /* denominator of b(p^e) : 5^e * e! */
        seq_t den_p = 1;
        for (unsigned j = 1; j <= e; ++j)
            den_p *= 5;                /* 5^e */
        seq_t fact = 1;
        for (unsigned j = 1; j <= e; ++j)
            fact *= j;                  /* e! */
        den_p *= fact;

        /* multiply into overall fraction */
        num *= num_p;
        den *= den_p;

        /* cancel any common factors immediately to keep the
           numbers small */
        seq_t g = gcd(num, den);
        if ((g > 1)) {                  /* g may be 0 only if num=den=0, impossible here */
            num /= g;
            den /= g;
        }
    }
    /* num now equals the numerator of b(n) after full reduction */
    return num;
}
