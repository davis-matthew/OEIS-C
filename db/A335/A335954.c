/*
 * A335954  a(n) = Numerator(-2*n*HurwitzZeta(1 - 2*n, -1/2)) for n > 0, and
 *          a(0) = 1.
 * https://oeis.org/A335954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Numerator(Bernoulli(2*n, -1/2)) a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   binom, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

typedef struct { seq_t num; seq_t den; } rational;

/* absolute value for seq_t */
static inline seq_t abs_seq_t(seq_t x) { return x < 0 ? -x : x; }
static inline seq_t gcd_seq_t(seq_t a, seq_t b) { return gcd(abs_seq_t(a), abs_seq_t(b)); }

/* add add_num/add_den to rational r (r = r + add_num/add_den) */
static void rational_add(rational *r, seq_t add_num, seq_t add_den)
{
    seq_t g = gcd_seq_t(r->den, add_den);
    seq_t mul1 = add_num * (r->den / g);
    seq_t mul2 = r->num * (add_den / g);
    seq_t new_num = mul1 + mul2;
    seq_t new_den = (r->den / g) * add_den;
    seq_t g2 = gcd_seq_t(new_num, new_den);
    r->num = new_num / g2;
    r->den = new_den / g2;
}

/* static storage for Bernoulli polynomials B_k(-1/2) */
static rational *B_poly  = NULL;   /* array of B_k(-1/2), k=0..max */
static idx_t   B_max    = 0;       /* highest computed index */

/* expand array and compute B_k(-1/2) up to i = new_max */
static void compute_B_polys(idx_t new_max)
{
    if (new_max <= B_max) return;
    size_t old_cap = new_max + 1;
    rational *tmp = (rational*)realloc(B_poly, old_cap * sizeof(rational));
    if (!tmp) abort();            /* memory error – program halt */
    B_poly = tmp;
    /* initialise B_0 */
    if (B_max == 0) {
        B_poly[0].num = 1;
        B_poly[0].den = 1;
    }
    /* compute B_k(-1/2) for k = current+1 .. new_max */
    for (idx_t i = B_max + 1; i <= new_max; ++i) {
        rational sum; sum.num = 0; sum.den = 1;
        for (idx_t k = 0; k < i; ++k) {
            seq_t coeff = binom((idx_t)(i + 1), k);        /* binom(i+1,k) */
            seq_t tnum  = coeff * B_poly[k].num;           /* numerator */
            seq_t tden  = B_poly[k].den;                   /* denominator */
            rational_add(&sum, tnum, tden);
        }
        /* B_i = - sum / (i+1)                                 */
        sum.den = sum.den * ((idx_t)(i + 1));              /* multiply den */
        sum.num = -sum.num;                                /* negate */
        seq_t g = gcd_seq_t(sum.num, sum.den);
        B_poly[i].num = sum.num / g;
        B_poly[i].den = sum.den / g;
        if (B_poly[i].num == 0) B_poly[i].den = 1;         /* normalise zero */
    }
    B_max = new_max;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* offset 0 */

    idx_t m = 2 * n;                      /* index of Bernoulli value */

    /* ensure Bernoulli polynomials up to index m are computed */
    compute_B_polys(m);

    seq_t Bnum = B_poly[m].num;   /* numerator of B_{2n}(-1/2) */
    seq_t Bden = B_poly[m].den;   /* denominator of B_{2n}(-1/2) */

    /* compute (2^{1-2n} - 1) = (1 - 2^{2n-1}) / 2^{2n-1} */
    seq_t pow2_M1 = pow_uint(2, m - 1);          /* 2^{2n-1}   */
    seq_t diff    = pow2_M1 - 1;                 /* 2^{2n-1}-1  */
    /* term1_num = -(diff * Bnum)  (rational with denominator pow2_M1 * Bden) */
    seq_t term1_num = - (diff * Bnum);

    /* second term: n / 2^{2n-2} */
    seq_t T = Bden * 2;          /* 2 * Bden    */
    seq_t term2_scaled = (seq_t)n * T;  /* numerator after scaling to common denominator */

    /* common denominator: pow2_M1 * Bden             */
    seq_t result_den = pow2_M1 * Bden;
    /* numerator: term1_num + term2_scaled            */
    seq_t result_num = term1_num + term2_scaled;

    /* reduce fraction to lowest terms and return numerator */
    seq_t g = gcd_seq_t(result_num, result_den);
    result_num = result_num / g;   /* numerator of the reduced fraction */

    return result_num;
}
