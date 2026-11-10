/*
 * A347396  a(n) = A347395(A276086(n)), where A347395 is Dirichlet convolution
 *          of Liouville's lambda with A342001.
 * https://oeis.org/A347396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{d | m} (-1)^(bigomega(m/d)) * A342001(d), where m =
 *            A276086(n); m's factorization comes straight from the
 *            primorial-base digits of n, and divisors are enumerated as
 *            exponent vectors
 * Range:     u8 n <= 245, u16 n <= 11550, u32 n <= 11550, u64 n <= 11550,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 110 terms through n = 9774 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A347396_MAXK 20

static const unsigned A347396_P[A347396_MAXK] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};

typedef struct {
    int k;
    int dig[A347396_MAXK];
    int e[A347396_MAXK];
    int total;
    seq_t pos, neg;
} A347396_ctx;

static void A347396_dfs(A347396_ctx *c, int i)
{
    if (i == c->k) {
        seq_t rad = 1;
        int om = 0;
        for (int j = 0; j < c->k; ++j) {
            if (c->e[j] > 0) { rad *= (seq_t)A347396_P[j]; om += c->e[j]; }
        }
        if (om == 0) return;
        seq_t val = 0;
        for (int j = 0; j < c->k; ++j) {
            if (c->e[j] > 0)
                val += (seq_t)c->e[j] * (rad / (seq_t)A347396_P[j]);
        }
        if (((c->total - om) & 1) == 0) c->pos += val;
        else c->neg += val;
        return;
    }
    for (int x = 0; x <= c->dig[i]; ++x) {
        c->e[i] = x;
        A347396_dfs(c, i + 1);
    }
    c->e[i] = 0;
}

seq_t a(idx_t n)
{
    A347396_ctx c;
    c.k = 0;
    c.total = 0;
    c.pos = 0;
    c.neg = 0;
    uint64_t r = (uint64_t)n;
    while (r > 0 && c.k < A347396_MAXK) {
        uint64_t p = A347396_P[c.k];
        c.dig[c.k] = (int)(r % p);
        c.e[c.k] = 0;
        c.total += c.dig[c.k];
        r /= p;
        c.k++;
    }
    if (c.k == 0) return 0;
    A347396_dfs(&c, 0);
    return c.pos - c.neg;
}
