/*
 * A354922  Positions of even terms in A182665.
 * https://oeis.org/A354922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A182665(k) is even; found by a
 *            sequential search over k, caching the terms found so far.
 *            A182665(k) = max over subsets S of the prime powers q_i of k of
 *            sum_{i in S} e_i mod k, where e_i = 1 mod q_i and 0 mod the
 *            other prime powers
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static int64_t A354922_egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) { *x = 1; *y = 0; return a; }
    int64_t x1, y1;
    int64_t g = A354922_egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

static uint64_t A354922_inv(uint64_t a, uint64_t m)
{
    int64_t x, y;
    A354922_egcd((int64_t)(a % m), (int64_t)m, &x, &y);
    x %= (int64_t)m;
    if (x < 0) x += (int64_t)m;
    return (uint64_t)x;
}

/* parity of A182665(k) */
static int A354922_parity(uint64_t k)
{
    if (k <= 1) return 0;
    uint64_t pw[64];
    size_t c = 0;
    uint64_t t = k;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t q = 1;
            while (t % p == 0) { t /= p; q *= p; }
            pw[c++] = q;
        }
    }
    if (t > 1) pw[c++] = t;
    uint64_t e[64];
    for (size_t i = 0; i < c; ++i) {
        uint64_t m = k / pw[i];
        uint64_t iv = A354922_inv(m % pw[i], pw[i]);
        e[i] = (uint64_t)((unsigned __int128)m * iv % k);
    }
    uint64_t best = 0;
    uint64_t masks = (uint64_t)1 << c;
    for (uint64_t mask = 0; mask < masks; ++mask) {
        unsigned __int128 s = 0;
        for (size_t i = 0; i < c; ++i)
            if (mask & ((uint64_t)1 << i)) s += e[i];
        uint64_t r = (uint64_t)(s % k);
        if (r > best) best = r;
    }
    return (int)(best & 1);
}

static uint64_t *A354922_terms = NULL;
static size_t A354922_cnt = 0, A354922_cap = 0;
static uint64_t A354922_next = 1;

seq_t a(idx_t n)
{
    while (A354922_cnt < (size_t)n) {
        uint64_t k = A354922_next++;
        if (A354922_parity(k) == 0) {
            if (A354922_cnt == A354922_cap) {
                size_t nc = A354922_cap ? A354922_cap * 2 : 1024;
                A354922_terms = (uint64_t *)realloc(A354922_terms, nc * sizeof(uint64_t));
                A354922_cap = nc;
            }
            A354922_terms[A354922_cnt++] = k;
        }
    }
    return (seq_t)A354922_terms[n - 1];
}
