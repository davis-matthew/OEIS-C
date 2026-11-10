/*
 * A389910  a(n) = min(n, A389909(n)).
 * https://oeis.org/A389910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(n, A389909(n)); A389909(n) = 2*A064989(n) for odd n,
 *            A003961(n/2) for even n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9903 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a389910_is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t a389910_next_prime(uint64_t n) {
    uint64_t p = n + 1;
    while (!a389910_is_prime(p)) ++p;
    return p;
}

static uint64_t a389910_prev_prime(uint64_t n) {
    uint64_t p = n - 1;
    while (!a389910_is_prime(p)) --p;
    return p;
}

/* Multiply res by f; return false if the product would exceed cap. */
static bool a389910_mul(uint64_t *res, uint64_t f, uint64_t cap) {
    if (f != 0 && *res > cap / f) return false;
    *res *= f;
    return *res <= cap;
}

/* Replace each prime factor p of m by its next (dir=1) or previous (dir=0)
   prime, multiplying onto *res; returns false if the result exceeds cap. */
static bool a389910_shift(uint64_t m, int dir, uint64_t *res, uint64_t cap) {
    uint64_t p = 2;
    while (p <= m / p) {
        if (m % p == 0) {
            uint64_t q = dir ? a389910_next_prime(p) : a389910_prev_prime(p);
            while (m % p == 0) {
                m /= p;
                if (!a389910_mul(res, q, cap)) return false;
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    if (m > 1) {
        uint64_t q = dir ? a389910_next_prime(m) : a389910_prev_prime(m);
        if (!a389910_mul(res, q, cap)) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t res = 1;
    bool ok;
    if ((N & 1ULL) == 0) {
        ok = a389910_shift(N >> 1, 1, &res, N);
    } else {
        res = 2;
        if (res > N) return (seq_t)N;
        ok = a389910_shift(N, 0, &res, N);
    }
    if (!ok || res > N) return (seq_t)N;
    return (seq_t)res;
}
