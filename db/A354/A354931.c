/*
 * A354931  a(n) = the least k for which A345992(k) = n.
 * https://oeis.org/A354931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A345992(k) = n (search over multiples k of
 *            n; A345992(k) = unitary divisor d of k minimizing the least m>0
 *            with d | m, (k/d) | m+1)
 * Range:     u8 n <= 17, u16 n <= 437, u32 n <= 3349, u64 n <= 3349,
 *            u128 n <= 3349, big any n
 * Validated: standard, pass, 82 terms through n = 3207 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static int64_t a354931_inv(int64_t a, int64_t mod) {
    int64_t r0 = mod, r1 = a % mod, t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = t0 - q * t1; t0 = t1; t1 = t;
    }
    t0 %= mod;
    if (t0 < 0) t0 += mod;
    return t0;
}

/* returns A345992(k) */
static uint64_t a354931_A345992(uint64_t k) {
    if (k == 1) return 1;
    uint64_t pp[20];
    size_t c = 0;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t pe = 1;
            while (x % p == 0) { x /= p; pe *= p; }
            pp[c++] = pe;
        }
    }
    if (x > 1) pp[c++] = x;
    uint64_t bestm = 0, bestd = 1;
    bool have = false;
    for (uint64_t mask = 0; mask < ((uint64_t)1 << c); ++mask) {
        uint64_t d = 1;
        for (size_t i = 0; i < c; ++i)
            if (mask & ((uint64_t)1 << i)) d *= pp[i];
        uint64_t e = k / d;
        uint64_t m;
        if (e == 1) {
            m = k;
        } else {
            uint64_t t = (uint64_t)a354931_inv((int64_t)(d % e), (int64_t)e);
            t = (e - t) % e;
            m = d * t;
        }
        if (!have || m < bestm) { have = true; bestm = m; bestd = d; }
    }
    return bestd;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    for (uint64_t j = 1;; ++j) {
        uint64_t k = nn * j;
        if (a354931_A345992(k) == nn) return (seq_t)k;
    }
}
