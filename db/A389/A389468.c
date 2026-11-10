/*
 * A389468  Irregular triangle T(n, k) read by rows, giving on row n all
 *          A389467(n) natural numbers in range [n, A003961(n)-1] that are
 *          coprime to n.
 * https://oeis.org/A389468
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   flatten rows m = 3,4,...; row m lists x in [m, A003961(m)-1]
 *            with gcd(x,m)=1, in increasing order; a(n) is the (n-2)-th entry
 *            overall (direct scan)
 * Range:     u8 n <= 924, u16 n <= 10490, u32 n <= 10490, u64 n <= 10490,
 *            u128 n <= 10490, big any n
 * Validated: standard, pass, 97 terms through n = 8309 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A389468_A003961(uint64_t n) {
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) result *= next_prime(m);
    return result;
}

static uint64_t A389468_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n) {
    uint64_t rem = (uint64_t)n - 3;
    for (uint64_t m = 3;; m++) {
        uint64_t hi = A389468_A003961(m);
        for (uint64_t x = m; x < hi; x++) {
            if (A389468_gcd(x, m) == 1) {
                if (rem == 0) return (seq_t)x;
                rem--;
            }
        }
    }
}
