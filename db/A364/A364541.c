/*
 * A364541  Numbers k for which A005940(k) <= k.
 * https://oeis.org/A364541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A005940(k) <= k, found by a search over
 *            k; A005940(k) is computed with a saturating check (exceeding k
 *            stops early)
 * Range:     u8 n <= 47, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void A364541_primes(uint64_t *p, int cnt) {
    int c = 0;
    uint64_t x = 2;
    while (c < cnt) {
        bool ok = true;
        for (int j = 0; j < c; j++) {
            if (p[j] * p[j] > x) break;
            if (x % p[j] == 0) { ok = false; break; }
        }
        if (ok) p[c++] = x;
        x++;
    }
}

static bool A364541_ok(uint64_t k, const uint64_t *pr) {
    uint64_t m = k - 1;
    uint64_t res = 1;
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            uint64_t p = pr[i];
            if (res > k / p) return false;
            res *= p;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return res <= k;
}

seq_t a(idx_t n) {
    uint64_t pr[80];
    A364541_primes(pr, 80);
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A364541_ok(k, pr)) count++;
    }
    return (seq_t)k;
}
