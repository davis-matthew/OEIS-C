/*
 * A340594  a(n) is the number of iterations of A340592 starting from n, until
 *          0, 1 or a prime is reached.
 * https://oeis.org/A340594
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066247(n)*(1 + a(A340592(n))), a(n) = 0 for prime n
 *            (iterate A340592 until 0, 1 or a prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 8444 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A340592_step(uint64_t n) {
    unsigned __int128 res = 0;
    uint64_t t = n;
    uint64_t p = 2;
    while (t > 1) {
        if ((unsigned __int128)p * p > t) p = t;
        if (t % p == 0) {
            unsigned d = 0;
            uint64_t q = p;
            while (q) { d++; q /= 10; }
            unsigned __int128 pw = 1 % n;
            for (unsigned i = 0; i < d; i++) pw = (pw * 10) % n;
            while (t % p == 0) {
                res = (res * pw + p % n) % n;
                t /= p;
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    return (uint64_t)res;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t count = 0;
    while (m > 1 && !is_prime_u64(m)) {
        m = A340592_step(m);
        count++;
        if (count > 10000000ULL) break;
    }
    return (seq_t)count;
}
