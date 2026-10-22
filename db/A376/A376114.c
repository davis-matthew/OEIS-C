/*
 * A376114  Refactorable numbers that are twice a square.
 * https://oeis.org/A376114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer of the form 2*s^2 such that
 *            tau(2*s^2) divides 2*s^2, where tau(n) is the number of divisors
 *            of n. (Intersection of A001105 (refactorable) and A033950 (twice
 *            a square).)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 4606, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* We search for the nth number of the form m = 2*s^2 with
       tau(m) | m.  We factor only s, which is considerably smaller than m,
       to compute the divisor count of m efficiently. */
    idx_t s = 1;          /* candidate square root */
    idx_t found = 0;      /* how many valid numbers we have seen */

    /* Temporary storage for prime factorization of s. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   prime_cnt;

    for (;; ++s) {
        /* m = 2 * s^2 */
        seq_t m = (seq_t)2 * (seq_t)s * (seq_t)s;

        /* Factor the small number s */
        factor_u64(s, primes, exps, &prime_cnt);

        /* Compute tau(2*s^2) = (2+2*e0) * Π (2*ei + 1)   (e0 = exponent of 2 in s) */
        seq_t tau = 2;      /* start with the factor coming from the ‘2’ multiplier */
        uint64_t e0 = 0;    /* exponent of 2 in s, if any */

        for (size_t i = 0; i < prime_cnt; ++i) {
            if (primes[i] == 2ULL) {
                e0 = exps[i];
            } else {
                /* other primes contribute (2*exp + 1) to tau */
                tau *= (seq_t)(2 * exps[i] + 1U);
            }
        }
        /* incorporate the exponent of 2 into tau */
        tau *= (seq_t)(e0 + 1U);

        /* Check the refactorable condition: tau divides m */
        if (m % tau == 0) {
            ++found;
            if (found == n) {
                return m;
            }
        }
    }
}
