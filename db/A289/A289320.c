/*
 * A289320  a(n) = A289310(n)^2 + A289311(n)^2.
 * https://oeis.org/A289320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^k∥n} (1 + p²)^k (totally multiplicative)
 * Range:     u8 n <= 14, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    if (n == 1)
        return result;          /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    uint64_t p = 2;

    while (p <= temp / p) {          /* trial division up to sqrt(temp) */
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t base = (seq_t)p * (seq_t)p + (seq_t)1;
            seq_t term = (seq_t)1;
            for (int i = 0; i < exp; ++i)
                term *= base;
            result *= term;
        }
        /* advance to next potential factor */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t base = (seq_t)temp * (seq_t)temp + (seq_t)1;
        result *= base;          /* exponent is 1 */
    }

    return result;
}
