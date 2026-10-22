/*
 * A372790  a(n) = tau(10*n) = A000005(10*n).
 * https://oeis.org/A372790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(10*n) – the number of divisors of 10·n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 10*n in 64‑bit arithmetic.  It is assumed that n is
       small enough that this multiplication does not overflow the
       64‑bit type; the limits used in the tests are far below this. */
    uint64_t m = n * 10ULL;

    seq_t divcnt = 1;           /* product of (exponent+1) */
    uint64_t exp, d;

    /* Factor 2 */
    if (m % 2 == 0) {
        exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++exp;
        }
        divcnt *= (seq_t)(exp + 1);
    }

    /* Factor odd primes */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            divcnt *= (seq_t)(exp + 1);
        }
    }

    /* If anything remains, it is a prime > sqrt(original m) */
    if (m > 1) {
        divcnt *= 2;   /* exponent 1 ⇒ (1+1) = 2 */
    }

    return divcnt;
}
