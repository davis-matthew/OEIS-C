/*
 * A390803  The sum of the integers k from 1 to n such that gcd(n, k) is a
 *          5-rough number (A007310).
 * https://oeis.org/A390803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n, gcd(k,n) is in A007310} k
 * Range:     u8 n <= 22, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is5rough
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is5rough.h"

/* Euler’s totient for 64‑bit integers. */
static inline uint64_t phi_u64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t result = n;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        uint64_t d64 = d;
        uint64_t m64 = n / d;
        if (is5rough(d64)) {
            seq_t contrib;
            if (m64 == 1) {
                contrib = (seq_t)d64;
            } else {
                uint64_t phi_m = phi_u64(m64);
                contrib = (seq_t)d64 *
                          (((seq_t)m64 * (seq_t)phi_m) / 2);
            }
            sum += contrib;
        }

        idx_t d2 = n / d;
        if (d2 == d) continue;          /* pair already handled */

        uint64_t d2_64 = d2;
        uint64_t m2_64 = n / d2;
        if (is5rough(d2_64)) {
            seq_t contrib;
            if (m2_64 == 1) {
                contrib = (seq_t)d2_64;
            } else {
                uint64_t phi_m = phi_u64(m2_64);
                contrib = (seq_t)d2_64 *
                          (((seq_t)m2_64 * (seq_t)phi_m) / 2);
            }
            sum += contrib;
        }
    }
    return sum;
}
