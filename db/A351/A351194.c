/*
 * A351194  Sum of the 6th powers of the primes dividing n.
 * https://oeis.org/A351194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t sum = 0;
    uint64_t tmp = n;

    /* Handle prime factor 2 */
    if (tmp % 2 == 0) {
        uint64_t p0 = 2;
        seq_t p = (seq_t)p0;
        seq_t p6 = p;
        p6 *= p; /* p^2 */
        p6 *= p; /* p^3 */
        p6 *= p; /* p^4 */
        p6 *= p; /* p^5 */
        p6 *= p; /* p^6 */
        sum += p6;
        while (tmp % 2 == 0) tmp /= 2;
    }

    /* Handle odd prime factors */
    for (uint64_t p0 = 3; p0 * p0 <= tmp; p0 += 2) {
        if (tmp % p0 == 0) {
            seq_t p = (seq_t)p0;
            seq_t p6 = p;
            p6 *= p; /* p^2 */
            p6 *= p; /* p^3 */
            p6 *= p; /* p^4 */
            p6 *= p; /* p^5 */
            p6 *= p; /* p^6 */
            sum += p6;
            while (tmp % p0 == 0) tmp /= p0;
        }
    }

    /* Remaining prime factor greater than sqrt(n) */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;
        seq_t p6 = p;
        p6 *= p; /* p^2 */
        p6 *= p; /* p^3 */
        p6 *= p; /* p^4 */
        p6 *= p; /* p^5 */
        p6 *= p; /* p^6 */
        sum += p6;
    }

    return sum;
}
