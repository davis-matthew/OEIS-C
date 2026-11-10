/*
 * A244412  Least even k such that sfdf(k-1) > sfdf(k-3) >= A050376(n), where
 *          sfdf(n) is the smallest Fermi-Dirac factor of n (A223490), and k-3
 *          is not the lesser of a pair of Fermi-Dirac twin primes (A229064).
 * https://oeis.org/A244412
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the least even k >= 4 such that k-3 > 1 is not
 *            in A050376 (so it is also not in A229064), A223490(k-3) >=
 *            A050376(n), and A223490(k-1) > A223490(k-3)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 44 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A244412_is_prime(uint64_t m) {
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A244412_isqrt(uint64_t m) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > m) bit >>= 2;
    while (bit) {
        if (m >= r + bit) { m -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* is p^(2^j), p prime */
static bool A244412_is_fd(uint64_t m) {
    if (m < 2) return false;
    for (;;) {
        if (A244412_is_prime(m)) return true;
        uint64_t r = A244412_isqrt(m);
        if (r * r != m) return false;
        m = r;
    }
}

/* smallest Fermi-Dirac factor */
static uint64_t A244412_sfdf(uint64_t m) {
    if (m <= 1) return 1;
    uint64_t best = UINT64_MAX;
    uint64_t d = 2;
    while (d * d <= m) {
        if (m % d == 0) {
            unsigned e = 0;
            while (m % d == 0) { m /= d; e++; }
            unsigned lb = e & (~e + 1u);
            uint64_t f = 1;
            for (unsigned i = 0; i < lb; i++) f *= d;
            if (f < best) best = f;
        }
        d += (d == 2) ? 1 : 2;
    }
    if (m > 1 && m < best) best = m;
    return best;
}

seq_t a(idx_t n) {
    /* t = n-th Fermi-Dirac prime */
    uint64_t cnt = 0, t = 1;
    while (cnt < (uint64_t)n) {
        t++;
        if (A244412_is_fd(t)) cnt++;
    }
    for (uint64_t k = 4;; k += 2) {
        uint64_t m = k - 3;
        if (m <= 1 || A244412_is_fd(m)) continue;
        uint64_t s3 = A244412_sfdf(m);
        if (s3 < t) continue;
        uint64_t s1 = A244412_sfdf(k - 1);
        if (!(s1 > s3)) continue;
        return (seq_t)k;
    }
}
