/*
 * A347103  G.f.: Sum_{k>=1} k * x^prime(k) / (1 + x^prime(k)).
 * https://oeis.org/A347103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{p|n, p prime} ( (n/p odd) ? pi(p) : -pi(p) )
 * Range:     i8 n <= 718, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9504 (2026-11-01)
 * Helpers:   prime_pi, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* n = 1 gives 0, n = 0 is graceful */
    seq_t result = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = smallest_prime_factor(temp);
        if (p == 0) {             /* safety: should not occur for temp > 1 */
            p = temp;
        }
        seq_t pi = prime_pi(p);
        /* parity of n / p */
        if (((n / p) & 1ULL) != 0ULL) {
            result += pi;         /* odd -> add pi(p) */
        } else {
            result -= pi;         /* even -> subtract pi(p) */
        }
        /* remove all copies of p from temp */
        while (temp % p == 0) {
            temp /= p;
        }
    }
    return result;
}
