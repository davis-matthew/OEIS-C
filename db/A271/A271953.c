/*
 * A271953  a(n) is the period of A000930 modulo n.
 * https://oeis.org/A271953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(a(p1^e1), ..., a(pk^ek)) over the prime factorization
 *            of n, where a(q) is the period of A000930 mod q, found by
 *            iterating the recurrence A000930(k) = A000930(k-1) +
 *            A000930(k-3), with initial values 1, 1, 1, until the state
 *            (1,1,1) recurs
 * Range:     u8 n <= 13, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 937 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A271953_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t A271953_period(uint64_t m) {
    if (m == 1) return 1;
    uint64_t x = 1 % m, y = 1 % m, z = 1 % m;
    uint64_t cnt = 0;
    do {
        uint64_t w = (z + x) % m;
        x = y;
        y = z;
        z = w;
        cnt++;
    } while (!(x == 1 % m && y == 1 % m && z == 1 % m));
    return cnt;
}

seq_t a(idx_t n) {
    seq_t res = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = 1;
            while (m % p == 0) {
                m /= p;
                q *= (uint64_t)p;
            }
            seq_t per = (seq_t)A271953_period(q);
            res = res / A271953_gcd(res, per) * per;
        }
    }
    if (m > 1) {
        seq_t per = (seq_t)A271953_period((uint64_t)m);
        res = res / A271953_gcd(res, per) * per;
    }
    return res;
}
