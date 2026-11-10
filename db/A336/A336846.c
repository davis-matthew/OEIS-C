/*
 * A336846  a(n) = gcd(sigma(A003961(n)), A000005(n)*A003961(n)).
 * https://oeis.org/A336846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(A003961(n)), A000005(n)*A003961(n)), where
 *            A003961 maps prime(k) -> prime(k+1)
 * Range:     u8 n <= 11, u16 n <= 1715, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 116 terms through n = 9575 (2026-11-08)
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t shifted = 1;   /* A003961(n) */
    seq_t sig = 1;       /* sigma(A003961(n)) */
    seq_t divs = 1;      /* d(n) */
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime((uint64_t)p);
            seq_t pw = 1, term = 1;
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
                pw *= q;
                term += pw;
                shifted *= q;
            }
            sig *= term;
            divs *= (seq_t)(e + 1);
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime((uint64_t)m);
        shifted *= q;
        sig *= (1 + q);
        divs *= 2;
    }
    return gcd(sig, divs * shifted);
}
