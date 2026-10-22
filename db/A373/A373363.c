/*
 * A373363  a(n) = gcd(A001414(n), A083345(n)), where A001414 is the sum of
 *          prime factors with repetition, and A083345 is the numerator of the
 *          sum of the inverses of prime factors with repetition.
 * https://oeis.org/A373363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001414(n), A083345(n)) where A001414(n) = sum of
 *            prime factors of n (with multiplicity), and A083345(n) =
 *            numerator of the sum of the inverses of prime factors of n (with
 *            multiplicity). The implementation performs prime factorisation
 *            with factor_u64, and keeps the rational sum of inve...
 * Range:     u8 n <= 513, u16 n <= 8866, u32 n <= 8866, u64 n <= 8866,
 *            u128 n <= 8866, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n = 1 the sum of prime factors is 0 and the rational
       sum of inverses is 0; conventionally a(1) = 0. */
    if (n < 2) return 0;

    /* Convert to uint64_t for factorisation; idx_t is guaranteed
       to be unsigned uint64_t. */
    uint64_t m = (uint64_t)n;

    seq_t sum_pf = 0;           /* A001414(n) */
    seq_t r_num  = 0;           /* numerator of A083345(n) */
    seq_t r_den  = 1;           /* denominator of A083345(n) */

    /* Buffer for prime factors and exponents.
       64 entries are more than sufficient for n ≤ 10 000. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    /* Factorise m.  The helper writes primes[] and exps[] and
       updates count with the number of distinct prime factors. */
    factor_u64(m, primes, exps, &count);

    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Update the sum of prime factors with multiplicity. */
        sum_pf += (seq_t)p * (seq_t)e;

        /* Update the rational sum r_num / r_den:
           r_num/r_den + e/p = (r_num*p + r_den*e) / (r_den*p) */
        seq_t new_num = r_num * (seq_t)p + r_den * (seq_t)e;
        seq_t new_den = r_den * (seq_t)p;

        /* Reduce the fraction to keep numbers small. */
        seq_t g = gcd(new_num, new_den);
        r_num = new_num / g;
        r_den = new_den / g;
    }

    /* a(n) is the gcd of the additive sum and the numerator of the
       rational sum of inverses. */
    return gcd(sum_pf, r_num);
}
