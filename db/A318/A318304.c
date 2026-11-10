/*
 * A318304  a(n) = A083254(n)/A003557(n) = (2*A173557(n) - A007947(n)).
 * https://oeis.org/A318304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A173557(n) - A007947(n) = 2*Product_{p|n}(p-1) -
 *            Product_{p|n} p
 * Range:     i8 n <= 130, i16 n <= 32770, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 9661 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t phi_rad = 1;  /* product of (p-1) over distinct primes p | n */
    seq_t rad = 1;      /* product of p over distinct primes p | n */
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            phi_rad *= (seq_t)(p - 1);
            rad *= (seq_t)p;
            do {
                m /= p;
            } while (m % p == 0);
        }
    }
    if (m > 1) {
        phi_rad *= (seq_t)(m - 1);
        rad *= (seq_t)m;
    }

    return (seq_t)2 * phi_rad - rad;
}
