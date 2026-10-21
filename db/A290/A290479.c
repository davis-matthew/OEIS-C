/*
 * A290479  Product of nonprime squarefree divisors of n.
 * https://oeis.org/A290479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n)^(d(rad(n))/2 - 1), where * rad(n) is the product
 *            of distinct primes dividing n, * d(m) = number of divisors (for
 *            square‑free m it's 2^k, k = #distinct primes). * For n a prime
 *            power, a(n) = 1
 * Range:     u8 n <= 29, u16 n <= 41, u32 n <= 209, u64 n <= 569,
 *            u128 n <= 2309, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Work in 64‑bit arithmetic for factorisation. */
    uint64_t tmp = (uint64_t)n;
    seq_t rad = (seq_t)1;        /* product of distinct primes */
    unsigned k   = 0;           /* number of distinct primes */

    /* Trial division by successive integers (fast enough for n ≤ 10000). */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            rad *= (seq_t)p;
            ++k;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {              /* remaining prime factor */
        rad *= (seq_t)tmp;
        ++k;
    }

    /* n = 1 or rad = 1 => a(n) = 1 */
    if (k == 0) return (seq_t)1;

    /* exponent = 2^(k-1) - 1  (k ≥ 1) */
    seq_t exp = ((seq_t)1 << (k - 1)) - (seq_t)1;

    /* For k = 1, exp = 0, so rad^0 = 1, which is correct. */
    return big_pow(rad, exp);
}
