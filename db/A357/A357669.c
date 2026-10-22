/*
 * A357669  a(n) is the number of divisors of the powerful part of n.
 * https://oeis.org/A357669
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

/* Sequence A357669: a(n) is the number of divisors of the powerful part of n */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 as the powerful part of 1 is 1 */
    if (n <= 1)
        return 1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* handle factor 2 */
    unsigned e = 0;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        e++;
    }
    if (e > 1)
        result *= (seq_t)(e + 1);

    /* handle odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2U) {
        if (m % p == 0U) {
            unsigned exp = 0;
            while (m % p == 0U) {
                m /= p;
                exp++;
            }
            if (exp > 1)
                result *= (seq_t)(exp + 1);
        }
    }

    /* any remaining prime factor has exponent 1 – contributes 1 to the product */

    return result;
}
