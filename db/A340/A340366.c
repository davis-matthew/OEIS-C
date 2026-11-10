/*
 * A340366  a(n) = A324106(n) / gcd(A005940(n), A324106(n)), where A324106(n)
 *          is multiplicative with a(p^e) = A005940(p^e).
 * https://oeis.org/A340366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A324106(n) / gcd(A005940(n), A324106(n)), A324106
 *            multiplicative with A324106(p^e) = A005940(p^e)
 * Range:     u8 n <= 56, u16 n <= 1472, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 118 terms through n = 9990 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint64_t A340366_nth_prime(unsigned k) {
    uint64_t c = 1;
    unsigned cnt = 0;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

static seq_t A340366_doudna(idx_t n) {
    idx_t m = n - 1;
    seq_t result = 1;
    unsigned i = 1;
    uint64_t p = 2;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)p;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
            p = A340366_nth_prime(i);
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t num = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t q = 1;
            while (m % p == 0) { m /= p; q *= p; }
            num *= A340366_doudna(q);
        }
    }
    if (m > 1) num *= A340366_doudna(m);
    seq_t d = A340366_doudna(n);
    seq_t g = gcd(d, num);
    return num / g;
}
