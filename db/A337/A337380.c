/*
 * A337380  a(n) = 1 if A003961(n) > 2*sigma(n), 0 otherwise.
 * https://oeis.org/A337380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003961(n) > 2*sigma(n), else 0; computed from the
 *            factorization of n (trial division)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9726 (2026-11-08)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    unsigned __int128 s = 1, r = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            unsigned __int128 term = 1, pp = 1;
            while (m % p == 0) {
                m /= p;
                pp *= p;
                term += pp;
                r *= q;
            }
            s *= term;
        }
    }
    if (m > 1) {
        s *= ((unsigned __int128)m + 1);
        r *= next_prime(m);
    }
    return (seq_t)(r > 2 * s ? 1 : 0);
}
