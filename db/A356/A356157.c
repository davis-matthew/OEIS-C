/*
 * A356157  The nearest common ancestor of sigma(n) and gcd(n, sigma(n)) in
 *          the Doudna tree (A005940).
 * https://oeis.org/A356157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = NCA(sigma(n), gcd(n, sigma(n))) in the Doudna tree, where
 *            parent(v) = A252463(v): v/2 if v is even, A064989(v) (each prime
 *            replaced by the previous prime) if v is odd; root 1. Computed by
 *            walking both nodes up to equal depth, then up together until
 *            they meet
 * Range:     u8 n <= 495, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9906 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A356157_sigma(uint64_t n) {
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t s = 1, pk = 1;
            while (n % p == 0) { n /= p; pk *= p; s += pk; }
            res *= s;
        }
    }
    if (n > 1) res *= (n + 1);
    return res;
}

static uint64_t A356157_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A356157_isprime(uint64_t m) {
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A356157_prevprime(uint64_t p) {
    uint64_t q = p - 1;
    while (q > 2 && !A356157_isprime(q)) q--;
    return q;
}

/* A252463: parent of v in the Doudna tree */
static uint64_t A356157_parent(uint64_t v) {
    if (v % 2 == 0) return v / 2;
    uint64_t res = 1;
    for (uint64_t p = 3; p * p <= v; p += 2) {
        if (v % p == 0) {
            uint64_t q = A356157_prevprime(p);
            while (v % p == 0) { v /= p; res *= q; }
        }
    }
    if (v > 1) res *= A356157_prevprime(v);
    return res;
}

static unsigned A356157_depth(uint64_t v) {
    unsigned d = 0;
    while (v > 1) { v = A356157_parent(v); d++; }
    return d;
}

seq_t a(idx_t n) {
    uint64_t x = A356157_sigma((uint64_t)n);
    uint64_t y = A356157_gcd((uint64_t)n, x);
    unsigned dx = A356157_depth(x);
    unsigned dy = A356157_depth(y);
    while (dx > dy) { x = A356157_parent(x); dx--; }
    while (dy > dx) { y = A356157_parent(y); dy--; }
    while (x != y) {
        x = A356157_parent(x);
        y = A356157_parent(y);
    }
    return (seq_t)x;
}
