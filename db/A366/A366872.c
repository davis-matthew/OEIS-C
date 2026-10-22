/*
 * A366872  Number of closed chains of identical regular polygons with
 *          connecting inner vertices lying n vertices apart.
 * https://oeis.org/A366872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(8+4n) (number of divisors of 8+4n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of positive divisors of 8+4*n.  This is
   an arithmetic function that finds all prime exponents of the
   integer m = 8 + 4*n and multiplies (exponent+1) for each prime. */
seq_t a(idx_t n)
{
    /* Compute the integer m = 8 + 4*n.  For the tested range of n
       (up to about 10 000) the result fits in 64 bits.  Use the
       128‑bit integer type in case n is larger, but cast back to
       64 bits for factoring. */
    __uint128_t m128 = (__uint128_t)4 * n + 8ULL;
    uint64_t m = (uint64_t)m128;

    uint64_t temp = m;
    seq_t result = (seq_t)1;

    /* Trial division by all integers up to sqrt(temp).  Since
       m <= 4·10⁴ + 8 for the relevant n, this loop is extremely fast. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result = result * (seq_t)(exp + 1);
        }
    }
    /* If temp > 1 then it is a prime factor larger than sqrt(original m). */
    if (temp > 1) {
        result = result * (seq_t)2;
    }
    return result;
}
