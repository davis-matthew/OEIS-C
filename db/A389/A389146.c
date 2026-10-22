/*
 * A389146  Classification of nonnegative integers into numbers in {0, 1},
 *          primes, proper prime powers, weak composites, and powerful numbers
 *          that are not prime powers.
 * https://oeis.org/A389146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=0; for n>=2, a(n)= 1 if n is prime, 2 if n is a proper
 *            prime power (p^k, k>1), 3 if n is a composite that is not
 *            powerful, 4 if n is a powerful number that is not a prime power
 *            Classification is done by factorising n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the special cases 0 and 1. */
    if (n < 2)
        return (seq_t)0;

    /* Factorise the number.  A 64‑bit integer can have at most 64
       distinct prime factors, so an on‑stack buffer of that size is
       sufficient. */
    uint64_t val = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt = 0;

    factor_u64(val, primes, exps, &cnt);

    /* Prime power?  If there is only one distinct prime factor it
       is a prime power.  Examine its exponent to distinguish a
       prime from a proper prime power. */
    if (cnt == 1) {
        return (exps[0] == 1) ? (seq_t)1 : (seq_t)2;
    }

    /* Composite case: determine whether all exponents are >= 2
       (i.e., whether the number is powerful). */
    bool all_ge2 = true;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 1) {
            all_ge2 = false;
            break;
        }
    }

    return all_ge2 ? (seq_t)4 : (seq_t)3;
}
