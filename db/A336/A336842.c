/*
 * A336842  Number of trailing 1-bits in the binary representation of
 *          A003961(n): a(n) = A007814(1+A003961(n)).
 * https://oeis.org/A336842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(1+A003961(n)); A003961 is completely
 *            multiplicative with prime(k) -> prime(k+1). Product is computed
 *            mod 2^64, which is enough since only the low bits are needed
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9967 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    uint64_t result = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) {
        result *= next_prime(m);
    }
    uint64_t t = result + 1;
    if (t == 0) return (seq_t)64;
    unsigned c = 0;
    while ((t & 1) == 0) {
        c++;
        t >>= 1;
    }
    return (seq_t)c;
}
