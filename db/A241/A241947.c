/*
 * A241947  Numbers n for which A241927(n) = 2.
 * https://oeis.org/A241947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th number m with A241927(m) = 2; found by search over m
 *            (A241927 recomputed by direct search)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A241947_is_fd_prime(uint64_t x) {
    if (x <= 1) return false;
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            unsigned e = 0;
            while (x % d == 0) { x /= d; ++e; }
            if (x != 1) return false;
            return (e & (e - 1)) == 0;
        }
    }
    return true;
}

static uint64_t A241947_isqrt(uint64_t n) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

static bool A241947_is_fd_semiprime(uint64_t m, bool sq) {
    uint64_t s = A241947_isqrt(m);
    for (uint64_t d = 2; d <= s; ++d) {
        if (m % d != 0) continue;
        uint64_t e = m / d;
        if (d == e) continue;
        if (!A241947_is_fd_prime(d) || !A241947_is_fd_prime(e)) continue;
        if (sq && ((d & 1ULL) != (e & 1ULL))) continue;
        return true;
    }
    return false;
}

static bool A241947_is_two(uint64_t n) {
    uint64_t s = A241947_isqrt(n);
    bool sq = (s * s == n);
    for (uint64_t k = 1; k * k < n; ++k) {
        uint64_t m = n - k * k;
        if (m <= 1) continue;
        if (A241947_is_fd_semiprime(m, sq)) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static uint64_t terms[64];
    static unsigned cnt = 0;
    static uint64_t next = 1;
    const uint64_t LIMIT = 200000;
    if (n < 1) return (seq_t)0;
    while (cnt < n && next <= LIMIT && cnt < 64) {
        if (A241947_is_two(next)) terms[cnt++] = next;
        ++next;
    }
    if (n <= cnt) return (seq_t)terms[n - 1];
    return (seq_t)0;
}
