/*
 * A295127  Numerator of Sum_{d|n} mu(n/d)/d, where mu is the Möbius function
 *          A008683.
 * https://oeis.org/A295127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, frac, easy
 * Formula:   a(n) = (-1)^ω(n) * ∏_{p|n}(p-1) / gcd(∏_{p|n}(p-1), n)
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t temp = n;
    seq_t prod = (seq_t)1;
    idx_t omega = 0;

    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        omega++;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }

    /* odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            omega++;
            while (temp % p == 0) temp /= p;
            prod *= (seq_t)(p - 1);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        omega++;
        prod *= (seq_t)(temp - 1);
    }

    seq_t g = gcd(prod, (seq_t)n);
    seq_t num = prod / g;
    if (omega & 1) num = -num;   /* apply the sign (-1)^ω(n) */
    return num;
}
