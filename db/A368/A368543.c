/*
 * A368543  The number of divisors of n whose prime factors are all of the
 *          form 2^k + 1 (A092506).
 * https://oeis.org/A368543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n, p∈{2,3,5,17,257,65537}} (e+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_fermat_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count divisors of n whose prime factors belong to {2}∪Fermat primes
 * (3,5,17,257,65537).  For each such prime factor p^e, multiply by e+1.
 * For other primes the factor contributes 1, i.e. only divisor 1.  The
 * function works for any positive n and any seq_t type.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_fermat_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset starts at 1, behaviour below is irrelevant */
    seq_t result = 1;
    uint64_t tmp = n;
    uint64_t exp = 0;

    /* Handle prime 2 */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;   /* divide by 2 */
        ++exp;
    }
    if (exp) result *= (seq_t)(exp + 1);

    /* Trial division for odd primes up to sqrt(tmp) */
    uint64_t p = 3;
    while (p <= tmp / p) {        /* ensures p*p <= tmp without overflow */
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (is_fermat_prime(p)) {
                result *= (seq_t)(exp + 1);
            }
        }
        p += 2; /* only odd numbers */
    }

    /* Remaining tmp > 1 is a prime factor */
    if (tmp > 1) {
        if (tmp == 2 || is_fermat_prime(tmp)) {
            result *= (seq_t)2; /* exponent is 1 -> (1+1) */
        }
    }

    return result;
}
