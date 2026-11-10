/*
 * A392621  Product of distinct integer roots r such that A001597(n) = r^m for
 *          some m > 1.
 * https://oeis.org/A392621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b^(sigma(g)-g) where A001597(n) = b^g, g = gcd of prime
 *            exponents (b not a perfect power); a(1) = 1
 * Range:     u8 n <= 35, u16 n <= 81, u32 n <= 4368, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9566 (2026-11-10)
 * Helpers:   mobius_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius_i32.h"

static bool A001597_pow_le(uint64_t b, int k, uint64_t x) {
    uint64_t r = 1;
    for (int i = 0; i < k; i++) {
        if (r > x / b) return false;
        r *= b;
    }
    return true;
}

static uint64_t A001597_iroot(uint64_t x, int k) {
    int sh = 64 / k + 1;
    uint64_t lo = 1, hi = (sh >= 63) ? ((uint64_t)1 << 33) : ((uint64_t)1 << sh);
    if (x == 0) return 0;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (A001597_pow_le(mid, k, x)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static uint64_t A001597_count(uint64_t x) {
    long long c = 1;
    for (int k = 2; k < 64; k++) {
        uint64_t r = A001597_iroot(x, k);
        if (r < 2) break;
        c -= (long long)mobius_i32(k) * (long long)(r - 1);
    }
    return (uint64_t)c;
}

static uint64_t A001597_val(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t lo = 1, hi = n * n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A001597_count(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

static unsigned A392621_gcd(unsigned a, unsigned b) {
    while (b) { unsigned t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n) {
    uint64_t s = A001597_val((uint64_t)n);
    if (s <= 1) return (seq_t)1;
    uint64_t ps[20];
    unsigned es[20];
    int cnt = 0;
    uint64_t m = s;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            ps[cnt] = p; es[cnt] = e; cnt++;
        }
    }
    if (m > 1) { ps[cnt] = m; es[cnt] = 1; cnt++; }
    unsigned g = 0;
    for (int i = 0; i < cnt; i++) g = A392621_gcd(g, es[i]);
    unsigned sg = 0;
    for (unsigned d = 1; d <= g; d++) if (g % d == 0) sg += d;
    unsigned E = sg - g;
    seq_t res = 1;
    for (int i = 0; i < cnt; i++) {
        unsigned k = (es[i] / g) * E;
        for (unsigned j = 0; j < k; j++) res *= (seq_t)ps[i];
    }
    return res;
}
