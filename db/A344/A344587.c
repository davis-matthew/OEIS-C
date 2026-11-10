/*
 * A344587  Deficiency of prime-shifted n: a(n) = 2*A003961(n) -
 *          sigma(A003961(n)).
 * https://oeis.org/A344587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 2*A003961(n) - sigma(A003961(n)), A003961 = prime-shift
 *            (p_k -> p_{k+1}), sigma(A003961(n)) = A003973(n)
 * Range:     i8 n <= 62, i16 n <= 2047, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 94 terms through n = 9313 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t shifted = 1;   /* A003961(n) */
    seq_t sig = 1;       /* A003973(n) */
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            seq_t power = 1;
            seq_t gsum = 1;
            while (m % p == 0) {
                m /= p;
                power *= q;
                gsum += power;
            }
            shifted *= power;
            sig *= gsum;
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime(m);
        shifted *= q;
        sig *= (1 + q);
    }
    return (seq_t)2 * shifted - sig;
}
