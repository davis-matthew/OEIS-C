/*
 * A368336  The number of divisors of the largest term of A072873 that divides
 *          of n.
 * https://oeis.org/A368336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A368336
    Formula: a(n) = multiplicative with a(p^e) = e - (e mod p) + 1
    a(n) = number of divisors of the largest term of A072873 that divides n.
    The function uses direct multiplicative evaluation from the prime factorisation of n.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 */
    if (tmp & 1ULL) {
        /* nothing */
    } else {
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        seq_t f = (seq_t)(e - (e % 2ULL) + 1ULL);
        result *= f;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t f = (seq_t)(e - (e % p) + 1ULL);
            result *= f;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        /* exponent e = 1 -> multiplier 1, but multiply to keep code simple */
        seq_t f = (seq_t)(1ULL - (1ULL % tmp) + 1ULL); /* equals 1 */
        result *= f;
    }

    return result;
}
