/*
 * A285587  Expansion of v * (v^4 - 3*v^3 + 4*v^2 - 2*v + 1) in powers of q
 *          where r() is the Rogers-Ramanujan continued fraction and v =
 *          r(q^5).
 * https://oeis.org/A285587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in v*(v^4-3*v^3+4*v^2-2*v+1), where v
 *            = r(q^5) and r(q) = q^(1/5) * (1 - q^4)(1 - q^9)(1 - q^14)... /
 *            (1 - q^2)(1 - q^7)(1 - q^12)... (product modulo 5)
 * Range:     i8 n <= 38, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   div_by_factor, mul_minus_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/div_by_factor.h"
#include "../utils/mul_minus_factor.h"

static seq_t *a_seq = NULL;     /* array of coefficients a(0..cap-1) */
static idx_t  a_cap = 0;        /* current capacity of a_seq */

/* convolution a*b modulo truncation to degree N */
static void conv(const seq_t *a, const seq_t *b, seq_t *c, idx_t N)
{
    /* reset c */
    for (idx_t i = 0; i <= N; ++i) c[i] = 0;
    for (idx_t i = 0; i <= N; ++i) {
        if (a[i] == 0) continue;
        for (idx_t j = 0; j + i <= N; ++j) {
            if (b[j] == 0) continue;
            c[i + j] += a[i] * b[j];
        }
    }
}

/* build A(x) up to index N and store in a_seq */
static void build_series(idx_t N)
{
    if (N >= a_cap) {               /* free previous storage */
        free(a_seq);
        a_seq = NULL;
        a_cap = 0;
    }

    a_cap = N + 1;
    a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));

    if (N == 0) {                   /* trivial case */
        a_seq[0] = 0;
        return;
    }

    /* ---------- compute v = q * P  ---------- */
    /* length of P is N (coefficients 0..N-1) because after shift we need up to N */
    seq_t *P = (seq_t *)calloc(N, sizeof(seq_t));
    P[0] = 1;                       /* start with constant term 1 */

    /* numerator factors: exponents 5+25k and 20+25k */
    for (idx_t m = 5; m < N; m += 25)   mul_minus_factor(P, N, m);
    for (idx_t m = 20; m < N; m += 25)  mul_minus_factor(P, N, m);

    /* denominator factors: exponents 10+25k and 15+25k  (division by (1-q^m)) */
    for (idx_t m = 10; m < N; m += 25)  div_by_factor(P, N, m);
    for (idx_t m = 15; m < N; m += 25)  div_by_factor(P, N, m);

    seq_t *v = (seq_t *)calloc(a_cap, sizeof(seq_t));
    v[0] = 0;                           /* constant term of v is 0 */
    for (idx_t i = 1; i <= N; ++i)      v[i] = P[i - 1];   /* shift by one */

    /* ---------- compute v^2, v^3, v^4, v^5 ---------- */
    seq_t *v2 = (seq_t *)calloc(a_cap, sizeof(seq_t));
    seq_t *v3 = (seq_t *)calloc(a_cap, sizeof(seq_t));
    seq_t *v4 = (seq_t *)calloc(a_cap, sizeof(seq_t));
    seq_t *v5 = (seq_t *)calloc(a_cap, sizeof(seq_t));

    conv(v, v, v2, N);          /* v^2 */
    conv(v2, v, v3, N);         /* v^3 */
    conv(v3, v, v4, N);         /* v^4 */
    conv(v4, v, v5, N);         /* v^5 */

    /* ---------- combine to form a(n) ---------- */
    for (idx_t i = 0; i <= N; ++i) {
        a_seq[i] = v5[i] - (3 * v4[i]) + (4 * v3[i]) - (2 * v2[i]) + v[i];
    }

    /* free temporary arrays */
    free(P);
    free(v);
    free(v2);
    free(v3);
    free(v4);
    free(v5);
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n >= a_cap) build_series(n);
    return a_seq[n];
}
