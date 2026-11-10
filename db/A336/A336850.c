/*
 * A336850  a(n) = gcd(A003961(n), sigma(A003961(n))), where A003961 is the
 *          prime shift towards larger primes.
 * https://oeis.org/A336850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003961(n), sigma(A003961(n))); A003961 shifts each
 *            prime of n to the next prime
 * Range:     u8 n <= 31, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9917 (2026-11-08)
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t m = 1;   /* A003961(n) */
    seq_t s = 1;   /* sigma(A003961(n)) */
    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            seq_t power = 1, sum = 1;
            while (t % p == 0) {
                t /= p;
                power = power * q;
                sum = sum + power;
            }
            m = m * power;
            s = s * sum;
        }
    }
    if (t > 1) {
        seq_t q = (seq_t)next_prime(t);
        m = m * q;
        s = s * (q + 1);
    }
    return gcd(m, s);
}
