/*
 * A321550  a(n) = Sum_{d|n} (-1)^(d-1)*d^11.
 * https://oeis.org/A321550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1)*d^11. The sequence is multiplicative
 *            with a(p^e) = 1 + p^11 + p^22 + ... + p^(11e) for odd p a(2^e) =
 *            1 - (2^11 + 2^22 + ... + 2^(11e)) The implementation expands
 *            these sums using only integer arithmetic
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 52, i128 n <= 2989,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64() */

seq_t a(idx_t n)
{
    if (n == 1)             /* a(1) = 1 by definition */
        return 1;

    /* Factor n: store primes and exponents in arrays */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p64 = primes[i];
        idx_t    e   = (idx_t)exps[i];
        seq_t    term = 1;          /* will hold a(p^e) */

        if (p64 == 2) {             /* special treatment for p=2 */
            /* Compute a(2^e) = 1 - (2^11 + 2^22 + ... + 2^(11e)) */
            seq_t   p11 = 1;
            for (idx_t k = 0; k < 11; ++k)  /* p11 = 2^11 = 2048 */
                p11 *= 2;

            seq_t sum = 0;
            seq_t exp_val = p11;     /* 2^11 for the first term */

            for (idx_t j = 1; j <= e; ++j) {
                sum += exp_val;      /* add current 2^(11*j) term */
                exp_val *= p11;      /* prepare next 2^(11*(j+1)) */
            }
            term = 1 - sum;
        } else {                      /* odd prime p */
            /* Compute a(p^e) = 1 + p^11 + p^22 + ... + p^(11e) */
            seq_t p11 = 1;
            for (idx_t k = 0; k < 11; ++k)
                p11 *= (seq_t)p64;   /* p^11 */

            seq_t mult = 1;          /* will hold p^(11*j) */
            for (idx_t j = 1; j <= e; ++j) {
                mult *= p11;         /* p^(11*j) */
                term += mult;        /* accumulate the series */
            }
        }

        result *= term;               /* combine multiplicatively */
    }

    return result;
}
