/*
 * A252748  a(n) = A003961(n) - 2*n.
 * https://oeis.org/A252748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003961(n) - 2*n, where A003961 is completely
 *            multiplicative with a(prime(k)) = prime(k+1)
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9799 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A003961_a(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime(m);
    }
    return result;
}

seq_t a(idx_t n) {
    return A003961_a(n) - (seq_t)2 * (seq_t)n;
}
