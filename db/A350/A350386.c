/*
 * A350386  a(n) is the sum of the even exponents in the prime factorization
 *          of n.
 * https://oeis.org/A350386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of even exponents in the prime factorization of n.
 *            For each prime p^e || n, contribute e if e is even, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* 1 and below have no prime factors */

    uint64_t m = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* handle factor 2 separately */
    uint8_t exp = 0;
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp && (exp & 1U) == 0) sum += (seq_t)exp;

    /* odd factors */
    uint64_t d = 3;
    while (d <= m / d) {                  /* avoid overflow: d*d <= m */
        exp = 0;
        while (m % d == 0) {
            exp++;
            m /= d;
        }
        if (exp && (exp & 1U) == 0) sum += (seq_t)exp;
        d += 2;
    }

    /* m now is either 1 or a prime > sqrt(original n).  Its exponent is 1 (odd). */
    return sum;
}
