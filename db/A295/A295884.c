/*
 * A295884  Number of exponents larger than 3 in the prime factorization of n.
 * https://oeis.org/A295884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of exponents > 3 in the prime factorization of n
 * Range:     u8 n <= 9964, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed by trial division, counting exponents > 3. */

#include <stdint.h>   /* for uint64_t used inside helper arrays */

/* The function uses only standard integer operations and no external
   helper functions, so it works for any seq_t/idx_t definition. */
seq_t a(idx_t n)
{
    /* Guard against the undefined behaviour for iterator below 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t res = 0;          /* Result to return */
    uint64_t temp = (uint64_t)n;   /* Operate on a native integer */

    /* Count exponent for prime 2 */
    if (temp % 2 == 0) {
        uint64_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++e;
        }
        if (e > 3) {
            ++res;
        }
    }

    /* Count exponents for odd prime factors */
    for (uint64_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            if (e > 3) {
                ++res;
            }
        }
    }

    /* Remaining temp > 1 is a prime factor with exponent 1 */
    /* Exponent 1 is never > 3, so nothing to add */

    return res;
}
