/*
 * A398637  Number of smooth points modulo n on the affine Cayley cubic
 *          x^2+y^2+z^2-x*y*z = 4.
 * https://oeis.org/A398637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * Π_{p|n, p odd prime} (1 - 3/p^2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(2^e) = 2^(2e), a(p^e) = p^(2e-2)*(p^2-3) for odd primes p */

#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = (seq_t)n * (seq_t)n;          /* n^2  */
    uint64_t temp = n;

    /* remove factor 2 (does not enter product) */
    while ((temp & 1) == 0)
        temp >>= 1;

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        /* remove all copies of this prime, but product uses distinct primes only */
        while (temp % p == 0)
            temp /= p;
        seq_t p2 = (seq_t)p * (seq_t)p;         /* p^2  */
        seq_t factor = p2 - 3;                  /* p^2 - 3  */
        result = result * factor / p2;          /* exact division */
    }

    if (temp > 1) { /* remaining prime factor */
        seq_t p2 = (seq_t)temp * (seq_t)temp;
        seq_t factor = p2 - 3;
        result = result * factor / p2;
    }

    return result;
}
