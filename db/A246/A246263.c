/*
 * A246263  Numbers n such that A003961(n) is of the form 4k+3.
 * https://oeis.org/A246263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A003961(m) == 3 (mod 4); found by
 *            search, testing each m by factoring and multiplying
 *            next_prime(p)^e mod 4
 * Range:     u8 n <= 131, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 86 terms through n = 9943 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A246263_ok(uint64_t m) {
    uint64_t r = 1; /* product mod 4 */
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p) & 3;
            while (m % p == 0) {
                m /= p;
                r = (r * q) & 3;
            }
        }
    }
    if (m > 1) {
        r = (r * (next_prime(m) & 3)) & 3;
    }
    return r == 3;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m = 0;
    while (count < (uint64_t)n) {
        m++;
        if (A246263_ok(m)) count++;
    }
    return (seq_t)m;
}
