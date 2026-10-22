/*
 * A325825  Square array giving the monic polynomial q satisfying q =
 *          gcd(P(x),P(y)) where P(x) and P(y) are polynomials in ring
 *          GF(3)[X] with coefficients in {0,1,2} given by the ternary
 *          expansions of x and y. The polynomial q is converted back to a
 *          ternary number, and then expressed in decimal.
 * https://oeis.org/A325825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gf3_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in A325825: 
 *  a(n) is the decimal value of the monic greatest common divisor
 *  of the polynomials P(x) and P(y) over GF(3), where (x,y)
 *  are obtained from n by reading the square array by antidiagonals. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gf3_inv.h"

/* small polynomial over GF(3) */
typedef struct {
    uint8_t c[64];      /* coefficients, least significant first */
    size_t len;         /* number of coefficients used (degree+1) */
} poly_t;

/* remove trailing zero coefficients */
static void poly_normalize(poly_t *p)
{
    while (p->len > 0 && p->c[p->len - 1] == 0)
        p->len--;
}

/* scale all coefficients by mul (1 or 2) */
static void poly_mul_scalar(poly_t *p, uint8_t mul)
{
    for (size_t i = 0; i < p->len; i++)
        p->c[i] = (p->c[i] * mul) % 3;
}

/* convert integer val to polynomial over GF(3) */
static void poly_from_uint64(uint64_t val, poly_t *p)
{
    if (val == 0) {
        p->len = 1;
        p->c[0] = 0;
        return;
    }
    p->len = 0;
    while (val > 0) {
        p->c[p->len++] = val % 3;
        val /= 3;
    }
}

/* remainder of a divided by b; result stored in rem (a<b in degree after loop) */
static void poly_mod(const poly_t *a, const poly_t *b, poly_t *rem)
{
    rem->len = a->len;
    for (size_t i = 0; i < a->len; i++)
        rem->c[i] = a->c[i];
    poly_normalize(rem);

    if (b->len == 0)
        return; /* division by zero, should not happen */

    size_t deg_b = b->len - 1;
    uint8_t inv_lead_b = gf3_inv(b->c[deg_b]);

    while (rem->len > 0) {
        size_t deg_r = rem->len - 1;
        if (deg_r < deg_b)
            break;
        size_t shift = deg_r - deg_b;
        uint8_t lead_r = rem->c[deg_r];
        uint8_t factor = (lead_r * inv_lead_b) % 3;

        for (size_t i = 0; i <= deg_b; i++) {
            size_t idx = shift + i;
            uint8_t sub = (b->c[i] * factor) % 3;
            rem->c[idx] = (rem->c[idx] + 3 - sub) % 3; /* subtraction in GF(3) */
        }
        poly_normalize(rem);
    }
}

/* Euclidean algorithm for polynomials over GF(3) */
static void poly_gcd(const poly_t *p, const poly_t *q, poly_t *g)
{
    poly_t a = *p, b = *q, r;
    while (b.len > 0) {
        poly_mod(&a, &b, &r);
        a = b;
        b = r;
    }
    if (a.len == 0) {            /* shouldn't occur for positive inputs */
        g->len = 0;
        return;
    }
    /* make leading coefficient 1 (monic) */
    uint8_t inv = gf3_inv(a.c[a.len - 1]);
    poly_mul_scalar(&a, inv);
    *g = a;
}

/* map index n to coordinates (x,y) in the square array */
static void antidiagonal_coords(idx_t n, idx_t *x, idx_t *y)
{
    idx_t k = 1;
    idx_t tri = 1;
    while (tri < n) {            /* find smallest k with k(k+1)/2 >= n */
        k++;
        tri += k;
    }
    idx_t prev = tri - k;        /* sum of first k-1 diagonals */
    idx_t offset = n - prev - 1; /* zero‑based offset inside diagonal k */
    *x = offset + 1;             /* columns */
    *y = k - offset;             /* rows */
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t x, y;
    antidiagonal_coords(n, &x, &y);

    poly_t px, py, q;
    poly_from_uint64((uint64_t)x, &px);
    poly_from_uint64((uint64_t)y, &py);

    poly_gcd(&px, &py, &q);

    /* convert polynomial back to integer (base 3 to decimal) */
    uint64_t val = 0;
    for (size_t i = 0; i < q.len; i++) {
        /* digits are stored least significant first, so we process from most significant */
    }
    for (size_t i = q.len; i > 0; i--) {
        val = val * 3 + q.c[i - 1];
    }

    return (seq_t)val;
}
