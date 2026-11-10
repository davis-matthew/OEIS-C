/*
 * A348044  The nearest common ancestor of n and n^2 in the Doudna tree
 *          (A005940).
 * https://oeis.org/A348044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest common ancestor of n and n^2 in the Doudna tree
 *            (A005940); parent(v) = v/2 if v even, else v with each prime
 *            replaced by the previous prime; root is 1
 * Range:     u8 n <= 15, u16 n <= 1312, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9124 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A348044_isp(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A348044_prevp(uint64_t p) {
    uint64_t q = p - 1;
    while (q >= 2 && !A348044_isp(q)) q--;
    return q;
}

static seq_t A348044_parent(seq_t v) {
    if (v <= 1) return 1;
    if (v % 2 == 0) return v / 2;
    seq_t res = 1;
    seq_t d = 3;
    while (d * d <= v) {
        if (v % d == 0) {
            seq_t q = (seq_t)A348044_prevp((uint64_t)d);
            while (v % d == 0) {
                v /= d;
                res *= q;
            }
        }
        d += 2;
    }
    if (v > 1) res *= (seq_t)A348044_prevp((uint64_t)v);
    return res;
}

static unsigned A348044_depth(seq_t v) {
    unsigned k = 0;
    while (v > 1) {
        v = A348044_parent(v);
        k++;
    }
    return k;
}

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t y = (seq_t)n * (seq_t)n;
    unsigned dx = A348044_depth(x), dy = A348044_depth(y);
    while (dx > dy) { x = A348044_parent(x); dx--; }
    while (dy > dx) { y = A348044_parent(y); dy--; }
    while (x != y) {
        x = A348044_parent(x);
        y = A348044_parent(y);
    }
    return x;
}
