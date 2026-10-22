/*
 * A359472  a(n) = 1 if the product of exponents in the prime factorization of
 *          n is 3, otherwise 0.
 * https://oeis.org/A359472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the product of the exponents in the prime
 *            factorization of n equals 3, otherwise 0
 * Range:     u8 n <= 8977, u16 n <= 8977, u32 n <= 8977, u64 n <= 8977,
 *            u128 n <= 8977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;
    /* factor 2 separately */
    if ((temp & 1) == 0) {
        seq_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            exp++;
        }
        prod *= exp;
        if (prod > 3) {
            return (seq_t)0;
        }
    }

    /* loop over odd divisors using p <= temp/p to avoid overflow */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            prod *= exp;
            if (prod > 3) {
                return (seq_t)0;
            }
        }
    }

    /* any remaining factor is prime with exponent 1, multiply by 1 (no effect) */

    return (seq_t)(prod == (seq_t)3);
}
