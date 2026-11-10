/*
 * A325971  a(n) is the least k >= A007947(n) such that -n + k and
 *          (n-sigma(n))+k are relatively prime.
 * https://oeis.org/A325971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= A007947(n) with gcd(k-n, k+n-sigma(n)) = 1
 *            (search over k)
 * Range:     u8 n <= 254, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 106 terms through n = 9535 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A007947_rad(idx_t n) {
    seq_t result = 1;
    seq_t m = (seq_t)n;
    if (m % 2 == 0) {
        result *= 2;
        while (m % 2 == 0) m /= 2;
    }
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result *= m;
    return result;
}

static seq_t A000203_sigma(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) result *= ((seq_t)m + 1);
    return result;
}

static seq_t A325971_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t s = A000203_sigma(n);
    seq_t k = A007947_rad(n);
    for (;; k++) {
        seq_t x = (k >= N) ? (k - N) : (N - k);
        seq_t kn = k + N;
        seq_t y = (kn >= s) ? (kn - s) : (s - kn);
        if (A325971_gcd(x, y) == 1) return k;
    }
}
