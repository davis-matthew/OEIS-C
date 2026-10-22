/*
 * A304438  Coefficient of s(y) in p(|y|), where s is Schur functions, p is
 *          power-sum symmetric functions, y is the integer partition with
 *          Heinz number n, and |y| = Sum y_i.
 * https://oeis.org/A304438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(e) if n has a prime > 2 in its factorization, else
 *            a(n) = (-1)^(e+1); a(n)=0 if n is not a hook Heinz number. Here
 *            e is the exponent of 2 in n. The sequence is defined for n ≥ 1
 * Range:     i8 n <= 8608, i16 n <= 8608, i32 n <= 8608, i64 n <= 8608,
 *            i128 n <= 8608, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* by convention */
    idx_t temp = n;
    idx_t e = 0;                   /* exponent of 2 */
    idx_t bigOmega = 0;            /* total number of prime factors (with multiplicity) */
    idx_t distinct_gt2 = 0;        /* number of distinct primes > 2 */

    /* factor 2 */
    while ((temp & 1ULL) == 0) {
        ++e;
        temp >>= 1;
        ++bigOmega;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                ++bigOmega;
            }
            if (exp > 1) return 0;           /* repeated prime > 2 → not a hook */
            ++distinct_gt2;                  /* one distinct prime > 2 */
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        if (temp != 2) {
            ++distinct_gt2;
            ++bigOmega;                       /* exponent is 1 */
        }
    }

    if (distinct_gt2 > 1) return 0;            /* more than one distinct prime > 2 */

    /* compute sign */
    seq_t sign = 1;
    if (distinct_gt2 == 0) {                  /* only power of 2 */
        if ((e & 1) == 0) sign = -1;           /* e even → (-1)^(e+1) = -1 */
    } else {                                  /* at least one other prime (exponent 1) */
        if (e & 1) sign = -1;                 /* e odd → (-1)^e = -1 */
    }
    return sign;
}
