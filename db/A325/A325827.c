/*
 * A325827  a(n) = A325825(2*n, sigma(n)).
 * https://oeis.org/A325827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A325825(2*n, sigma(n)): the monic gcd over GF(3)[X] of
 *            the polynomials whose ternary digits are 2n and sigma(n), read
 *            back as a ternary number
 * Range:     u8 n <= 257, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9924 (2026-11-08)
 * Helpers:   sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma.h"

typedef struct {
    uint8_t c[256];
    size_t len;
} A325827_poly;

static void A325827_norm(A325827_poly *p)
{
    while (p->len > 0 && p->c[p->len - 1] == 0)
        p->len--;
}

static void A325827_from(seq_t v, A325827_poly *p)
{
    p->len = 0;
    while (v > 0) {
        p->c[p->len++] = (uint8_t)(v % 3);
        v /= 3;
    }
}

static void A325827_mod(const A325827_poly *a, const A325827_poly *b, A325827_poly *r)
{
    size_t i;
    *r = *a;
    A325827_norm(r);
    if (b->len == 0) return;
    size_t db = b->len - 1;
    uint8_t inv = b->c[db]; /* 1->1, 2->2 */
    while (r->len > 0 && r->len - 1 >= db) {
        size_t sh = r->len - 1 - db;
        uint8_t f = (uint8_t)((r->c[r->len - 1] * inv) % 3);
        for (i = 0; i <= db; i++) {
            uint8_t sub = (uint8_t)((b->c[i] * f) % 3);
            r->c[sh + i] = (uint8_t)((r->c[sh + i] + 3 - sub) % 3);
        }
        A325827_norm(r);
    }
}

seq_t a(idx_t n)
{
    A325827_poly x, y, r;
    A325827_from((seq_t)n * 2, &x);
    A325827_from(sigma(n), &y);
    while (y.len > 0) {
        A325827_mod(&x, &y, &r);
        x = y;
        y = r;
    }
    seq_t val = 0;
    if (x.len == 0) return val;
    uint8_t inv = x.c[x.len - 1];
    for (size_t i = x.len; i > 0; i--) {
        uint8_t d = (uint8_t)((x.c[i - 1] * inv) % 3);
        val = val * 3 + d;
    }
    return val;
}
