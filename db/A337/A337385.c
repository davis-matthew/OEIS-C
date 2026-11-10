/*
 * A337385  Odd numbers k for which A003973(k) >= 2*A003961(k).
 * https://oeis.org/A337385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A003973(k) >= 2*A003961(k); search over
 *            odd multiples of 255255 (3*5*7*11*13*17), as the OEIS comment
 *            says all terms are
 * Range:     u8 none, u16 none, u32 none, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A337385_test(uint64_t k) {
    seq_t t = 1, s = 1;
    uint64_t m = k;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t pw = 1, sum = 1;
            while (m % p == 0) {
                m /= p;
                t *= (seq_t)q;
                pw *= (seq_t)q;
                sum += pw;
            }
            s *= sum;
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        t *= (seq_t)q;
        s *= (seq_t)(1 + q);
    }
    return s >= 2 * t;
}

seq_t a(idx_t n) {
    static uint64_t cache[4096];
    static idx_t count = 0;
    static uint64_t next_m = 1;
    while (count < n && count < 4096) {
        uint64_t k = 255255ULL * next_m;
        next_m += 2;
        if (A337385_test(k)) {
            cache[count++] = k;
        }
    }
    return (seq_t)cache[n - 1];
}
