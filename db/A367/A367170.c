/*
 * A367170  The number of divisors of the largest unitary divisor of n that is
 *          a term of A138302.
 * https://oeis.org/A367170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e || n , e is a power of 2} (e+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* true iff x is a positive power of two */
static inline bool is_power_of_two_uint64(uint64_t x)
{
    return x && ((x & (x - 1)) == 0);
}

seq_t a(idx_t n)
{
    seq_t res = 1;

    if (n <= 1) {
        /* a(1) = 1 by definition */
        return res;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t e = 0;

    /* factor the two */
    while ((nn & 1ULL) == 0) {
        nn >>= 1;
        ++e;
    }
    if (e > 0 && is_power_of_two_uint64(e)) {
        res *= (seq_t)(e + 1);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            if (is_power_of_two_uint64(e)) {
                res *= (seq_t)(e + 1);
            }
        }
    }

    /* remaining prime factor with exponent 1, if any */
    if (nn > 1) {
        /* exponent is 1 (a power of two) */
        res *= (seq_t)2;          /* e + 1 = 2 */
    }

    return res;
}
