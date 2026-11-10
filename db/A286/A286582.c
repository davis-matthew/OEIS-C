/*
 * A286582  a(n) = A001222(A048673(n)).
 * https://oeis.org/A286582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A048673(n)), where A048673(n) = (A003961(n)+1)/2
 *            and A003961 shifts each prime factor of n to the next prime
 * Range:     u8 n <= 47, u16 n <= 7175, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9205 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A286582_bigomega(seq_t m) {
    seq_t c = 0;
    if (m < 2) return 0;
    while (m % 2 == 0) { c++; m /= 2; }
    for (seq_t i = 3; i <= m / i; i += 2) {
        while (m % i == 0) { c++; m /= i; }
    }
    if (m > 1) c++;
    return c;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;
    seq_t prod = 1;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            seq_t np = (seq_t)next_prime((uint64_t)p);
            while (t % p == 0) { t /= p; prod = prod * np; }
        }
    }
    if (t > 1) {
        seq_t np = (seq_t)next_prime((uint64_t)t);
        prod = prod * np;
    }
    return A286582_bigomega((prod + 1) / 2);
}
