/*
 * A396683  a(n) is the number of divisors of A006863(n).
 * https://oeis.org/A396683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(A006863(n)); with m = 2n, 2^e || m, this is (e+3)
 *            * Product_{p odd prime, (p-1) | m, p^b || m} (b+2); a(0) = 1
 * Range:     u8 n <= 17, u16 n <= 179, u32 n <= 3779, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9886 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static seq_t a_divcnt_factor(uint64_t m, uint64_t p) {
    uint64_t b = 0;
    while (m % p == 0) {
        b++;
        m /= p;
    }
    return (seq_t)(b + 2);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    uint64_t m = 2 * (uint64_t)n;
    uint64_t e = 0;
    uint64_t t = m;
    while (t % 2 == 0) {
        e++;
        t /= 2;
    }
    seq_t result = (seq_t)(e + 3);

    for (uint64_t d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            uint64_t p = d + 1;
            if (p > 2 && is_prime_u64(p)) result *= a_divcnt_factor(m, p);
            uint64_t d2 = m / d;
            if (d2 != d) {
                p = d2 + 1;
                if (p > 2 && is_prime_u64(p)) result *= a_divcnt_factor(m, p);
            }
        }
    }
    return result;
}
