/*
 * A356306  The nearest common ancestor of A000265(n) and gcd(A000265(n),
 *          sigma(n)) in the A253563-tree.
 * https://oeis.org/A356306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A356300(A000265(n), A355931(n)); A356300 = label of the
 *            nearest common ancestor in the A253563 tree (parent of v: v/L(v)
 *            if the top prime power of v has exponent >= 2, otherwise
 *            v/L(v)*prevprime(L(v)); parent of 2 is 1)
 * Range:     u8 n <= 4679, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9884 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A356306_u128;

static bool A356306_isprime(uint64_t m) {
    if (m < 2) return false;
    for (uint64_t d = 2; d <= m / d; ++d)
        if (m % d == 0) return false;
    return true;
}

static uint64_t A356306_parent(uint64_t v) {
    if (v <= 1) return 1;
    if (v == 2) return 1;
    uint64_t t = v, r = 1;
    int e = 0;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            r = p;
            e = 0;
            while (t % p == 0) { t /= p; ++e; }
        }
    }
    if (t > 1) { r = t; e = 1; }
    if (e >= 2) return v / r;
    uint64_t q = r - 1;
    while (!A356306_isprime(q)) --q;
    return v / r * q;
}

static int A356306_depth(uint64_t v) {
    int d = 0;
    while (v > 1) { v = A356306_parent(v); ++d; }
    return d;
}

static uint64_t A356306_nca(uint64_t x, uint64_t y) {
    int dx = A356306_depth(x), dy = A356306_depth(y);
    while (dx > dy) { x = A356306_parent(x); --dx; }
    while (dy > dx) { y = A356306_parent(y); --dy; }
    while (x != y) { x = A356306_parent(x); y = A356306_parent(y); }
    return x;
}

seq_t a(idx_t n) {
    uint64_t odd = (uint64_t)n;
    while (odd && (odd & 1ULL) == 0) odd >>= 1;
    A356306_u128 sigma = 1;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            A356306_u128 term = 1, pw = 1;
            while (temp % p == 0) {
                temp /= p;
                pw *= p;
                term += pw;
            }
            sigma *= term;
        }
    }
    if (temp > 1) sigma *= (1 + (A356306_u128)temp);
    A356306_u128 g = (A356306_u128)odd, h = sigma;
    while (h) { A356306_u128 t = g % h; g = h; h = t; }
    uint64_t y = (uint64_t)g;
    return (seq_t)A356306_nca(odd, y);
}
