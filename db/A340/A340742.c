/*
 * A340742  Primes in A340740, in the order in which they occur.
 * https://oeis.org/A340742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A340740(A340741(n)), i.e. the n-th prime among the terms
 *            of A340740, where A340740(m) = Sum_{k=1..floor((m-1)/2),
 *            gcd(m,k)=1} (m mod k); computed by scanning m upward with a
 *            cache, using Moebius inclusion-exclusion over blocks of equal
 *            floor(m/k)
 * Range:     u8 n <= 24, u16 n <= 130, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 81 terms through n = 4851 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A340742_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A340742_divs[2048];
static int A340742_sgn[2048];
static int A340742_nd;

static void A340742_setup(uint64_t m)
{
    uint64_t ps[16];
    int np = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            ps[np++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) ps[np++] = t;
    A340742_divs[0] = 1;
    A340742_sgn[0] = 1;
    int nd = 1;
    for (int i = 0; i < np; ++i) {
        for (int j = 0; j < nd; ++j) {
            A340742_divs[nd + j] = A340742_divs[j] * ps[i];
            A340742_sgn[nd + j] = -A340742_sgn[j];
        }
        nd *= 2;
    }
    A340742_nd = nd;
}

/* count and sum of integers in [1,x] coprime to m (mod 2^64) */
static void A340742_cs(uint64_t x, uint64_t *c, uint64_t *s)
{
    uint64_t cc = 0, ss = 0;
    for (int i = 0; i < A340742_nd; ++i) {
        uint64_t d = A340742_divs[i];
        uint64_t y = x / d;
        uint64_t tr = (y % 2 == 0) ? (y / 2) * (y + 1) : y * ((y + 1) / 2);
        if (A340742_sgn[i] > 0) { cc += y; ss += d * tr; }
        else { cc -= y; ss -= d * tr; }
    }
    *c = cc;
    *s = ss;
}

static uint64_t A340742_sum(uint64_t m)
{
    if (m < 3) return 0;
    uint64_t L = (m - 1) / 2;
    uint64_t B = 1;
    while ((B + 1) * (B + 1) <= m) ++B;
    uint64_t total = 0;
    uint64_t lim = B < L ? B : L;
    for (uint64_t k = 1; k <= lim; ++k)
        if (A340742_gcd(m, k) == 1) total += m % k;
    if (L <= B) return total;
    A340742_setup(m);
    uint64_t pc, ps;
    A340742_cs(B, &pc, &ps);
    uint64_t k = B + 1;
    while (k <= L) {
        uint64_t q = m / k;
        uint64_t hi = m / q;
        if (hi > L) hi = L;
        uint64_t c, s;
        A340742_cs(hi, &c, &s);
        total += m * (c - pc) - q * (s - ps);
        pc = c; ps = s;
        k = hi + 1;
    }
    return total;
}

static uint64_t *A340742_terms = 0;
static size_t A340742_cnt = 0, A340742_cap = 0;
static uint64_t A340742_next = 1;

seq_t a(idx_t n)
{
    while (A340742_cnt < (size_t)n) {
        uint64_t v = A340742_sum(A340742_next++);
        if (v >= 2 && is_prime_u64(v)) {
            if (A340742_cnt == A340742_cap) {
                A340742_cap = A340742_cap ? A340742_cap * 2 : 256;
                A340742_terms = (uint64_t *)realloc(A340742_terms, A340742_cap * sizeof(uint64_t));
            }
            A340742_terms[A340742_cnt++] = v;
        }
    }
    return (seq_t)A340742_terms[n - 1];
}
