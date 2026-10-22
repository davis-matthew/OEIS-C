/*
 * A387960  Multiplicative sequence a(n) with a(p^e) = 1 + (e mod 3) * (2 - (e
 *          mod 3)) for prime p and e >= 0.
 * https://oeis.org/A387960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^m, where m is the number of prime exponents congruent
 *            to 1 modulo 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;          /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    unsigned count = 0;

    /* factor 2 */
    unsigned e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e++;
    }
    if (e % 3 == 1) count++;

    /* odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (e % 3 == 1) count++;
    }

    /* remaining prime factor, if any, has exponent 1 */
    if (temp > 1) {
        /* 1 % 3 == 1, so always contributes */
        count++;
    }

    seq_t result = (seq_t)1;
    result <<= count;   /* multiply by 2^count */
    return result;
}
