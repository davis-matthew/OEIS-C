/*
 * A356301  The nearest common ancestor of A000265(sigma(n)) and A000265(n) in
 *          the tree depicted in A253563.
 * https://oeis.org/A356301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A356300(A161942(n), A000265(n)): nearest common ancestor
 *            in the tree of A253563 (parent of v: v/L(v) if L(v)^2 | v, else
 *            v/L(v)*prevprime(L(v)); L = largest prime factor; parent(2)=1)
 * Range:     u8 n <= 540, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9676 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ONE ((seq_t)1)

static seq_t A356301_lpf(seq_t x)
{
    seq_t r = ONE;
    seq_t d;
    for (d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            r = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > ONE) r = x;
    return r;
}

static bool A356301_isp(seq_t x)
{
    seq_t d;
    if (x < 2) return false;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static seq_t A356301_parent(seq_t v)
{
    if (v <= 2) return ONE;
    seq_t q = A356301_lpf(v);
    seq_t w = v / q;
    if (w % q == 0) return w;
    seq_t p = q - 1;
    while (!A356301_isp(p)) p--;
    return w * p;
}

static int A356301_depth(seq_t v)
{
    int d = 0;
    while (v > ONE) {
        v = A356301_parent(v);
        d++;
    }
    return d;
}

static seq_t A356301_sigma_odd(idx_t n)
{
    idx_t tmp = n;
    seq_t sigma = ONE;
    idx_t p;
    for (p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            seq_t pp = ONE, sum = ONE;
            while (tmp % p == 0) {
                tmp /= p;
                pp *= (seq_t)p;
                sum += pp;
            }
            sigma *= sum;
        }
    }
    if (tmp > 1) sigma *= ((seq_t)tmp + ONE);
    while ((sigma & ONE) == 0) sigma >>= 1;
    return sigma;
}

seq_t a(idx_t n)
{
    seq_t x = A356301_sigma_odd(n);
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;
    seq_t y = (seq_t)m;

    int dx = A356301_depth(x), dy = A356301_depth(y);
    while (dx > dy) { x = A356301_parent(x); dx--; }
    while (dy > dx) { y = A356301_parent(y); dy--; }
    while (x != y) {
        x = A356301_parent(x);
        y = A356301_parent(y);
    }
    return x;
}
