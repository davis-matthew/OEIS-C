/*
 * A356308  a(n) = gcd(n, A356301(n)), where A356301(n) is the nearest common
 *          ancestor of A000265(sigma(n)) and A000265(n) in the A253563-tree.
 * https://oeis.org/A356308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A356301(n)); A356301(n) = nearest common ancestor
 *            of A000265(sigma(n)) and A000265(n) in the A253563 tree (root 1,
 *            child 2; value x has children x*L(x) and x*N(L(x))/L(x), L =
 *            largest prime factor, N = next prime)
 * Range:     u8 n <= 822, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8918 (2026-11-10)
 * Helpers:   gcd, sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma.h"
#include "../utils/gcd.h"

static seq_t A356308_odd(seq_t x)
{
    if (x == 0) return x;
    while ((x & 1) == 0) x >>= 1;
    return x;
}

static seq_t A356308_lpf(seq_t x)
{
    seq_t r = 1;
    if (x <= 1) return x;
    while ((x & 1) == 0) { r = 2; x >>= 1; }
    for (seq_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            r = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) r = x;
    return r;
}

static bool A356308_isprime(seq_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (seq_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static seq_t A356308_prevprime(seq_t q)
{
    seq_t p = q - 1;
    while (p >= 2 && !A356308_isprime(p)) p--;
    return p;
}

/* parent of v in the tree of A253563 (v > 1) */
static seq_t A356308_parent(seq_t v)
{
    if (v <= 2) return 1;
    seq_t p = A356308_lpf(v);
    if (v % (p * p) == 0) return v / p;
    seq_t pp = A356308_prevprime(p);
    return v / p * pp;
}

static idx_t A356308_depth(seq_t v)
{
    idx_t d = 0;
    while (v > 1) { v = A356308_parent(v); d++; }
    return d;
}

seq_t a(idx_t n)
{
    seq_t x = A356308_odd(sigma(n));
    seq_t y = A356308_odd((seq_t)n);
    idx_t dx = A356308_depth(x), dy = A356308_depth(y);
    while (dx > dy) { x = A356308_parent(x); dx--; }
    while (dy > dx) { y = A356308_parent(y); dy--; }
    while (x != y) {
        x = A356308_parent(x);
        y = A356308_parent(y);
    }
    return gcd((seq_t)n, x);
}
