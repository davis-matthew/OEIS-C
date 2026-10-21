/*
 * A277002  Numerators of an asymptotic series for the Gamma function (odd
 *          power series).
 * https://oeis.org/A277002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of b(2*n-1) where b(k) = Bernoulli(k+1,
 *            1/2)/(k*(k+1)). Bernoulli(x, 1/2) = (2^(1-x) - 1) *
 *            Bernoulli(x). The function computes the sequence by generating
 *            Bernoulli numbers B_0 … B_{2n} via the recurrence sum_{k=0}^{m}
 *            binom(m+1, k) B_k = 0 (m ≥ 1) and then forms the required num...
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Rational number in lowest terms */
typedef struct {
    seq_t num;
    seq_t den;
} rational_t;

/* Reduce a rational number to lowest terms and keep denominator positive */
static inline void rational_normalize(rational_t *r)
{
    if (r->den < (seq_t)0) {
        r->den = -r->den;
        r->num = -r->num;
    }
    if (r->num == (seq_t)0) {
        r->den = (seq_t)1;
        return;
    }
    seq_t abs_num = (r->num >= (seq_t)0) ? r->num : -r->num;
    seq_t g = gcd(abs_num, r->den);
    if (g > (seq_t)1) {
        r->num /= g;
        r->den /= g;
    }
}

/* r = a + b */
static inline void rational_add(rational_t *r, const rational_t *a, const rational_t *b)
{
    r->num = a->num * b->den + b->num * a->den;
    r->den = a->den * b->den;
    rational_normalize(r);
}

/* r = a * m   (m is non‑negative integer) */
static inline void rational_mul_int(rational_t *r, idx_t m)
{
    if (m == 0) {
        r->num = (seq_t)0;
        r->den = (seq_t)1;
        return;
    }
    r->num *= (seq_t)m;
    /* Reduce common factors between m and denominator */
    seq_t g = gcd((seq_t)m, r->den);
    if (g > (seq_t)1) {
        r->num /= g;
        r->den /= g;
    }
    rational_normalize(r);
}

/* Global storage for Bernoulli numbers */
static rational_t *B = NULL;
static idx_t B_size = 0;

/* Ensure B[0 … n] are computed */
static void ensure_B(idx_t n)
{
    if (B_size > n) return;
    /* Allocate or grow */
    size_t new_size = (size_t)n + 1;
    rational_t *new_B = (rational_t *)realloc(B, new_size * sizeof(rational_t));
    if (!new_B) return;  /* allocation failure – let the caller handle */
    B = new_B;
    /* Initialise new elements to 0/1 */
    for (size_t i = B_size; i < new_size; ++i) {
        B[i].num = (seq_t)0;
        B[i].den = (seq_t)1;
    }
    /* Compute new Bernoulli numbers */
    for (size_t m = B_size; m <= n; ++m) {
        if (m == 0) {
            B[0].num = (seq_t)1;
            B[0].den = (seq_t)1;
            continue;
        }
        rational_t sum = { (seq_t)0, (seq_t)1 };
        for (size_t k = 0; k < m; ++k) {
            rational_t term = B[k];
            seq_t coeff = binomial((idx_t)(m + 1), (idx_t)k);
            term.num *= coeff;  /* term.num = term.num * coeff */
            rational_add(&sum, &sum, &term);
        }
        /* B_m = -sum / (m+1) */
        B[m].num = -sum.num;
        B[m].den = sum.den * (seq_t)(m + 1);
        rational_normalize(&B[m]);
    }
    B_size = new_size;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n<1 */
    idx_t m = (idx_t)2 * n - (idx_t)1;   /* odd index whose Bernoulli term matters */
    idx_t fill_needed = m + (idx_t)1;    /* need B[2n] = B[m+1] */
    ensure_B(fill_needed);

    /* b_m = (2^{-m} - 1) * B_{m+1} / (m*(m+1)) */
    rational_t *Btwo = &B[m + 1];
    /* Compute 2^m */
    seq_t two_pow_m = big_pow((seq_t)2, (seq_t)m);
    seq_t num_factor = two_pow_m - (seq_t)1;   /* 2^m - 1 */
    /* Numerator: -num_factor * B_{2n}.num */
    seq_t numerator = - (num_factor * Btwo->num);
    /* Denominator: 2^m * B_{2n}.den * m*(m+1) */
    seq_t denominator = two_pow_m * Btwo->den;
    denominator *= (seq_t)m;
    denominator *= (seq_t)(m + (idx_t)1);

    /* Reduce the fraction */
    seq_t abs_num = (numerator >= (seq_t)0) ? numerator : -numerator;
    seq_t g = gcd(abs_num, denominator);
    if (g > (seq_t)1) {
        numerator /= g;
        /* denominator /= g; not needed after this point */
    }
    return numerator;
}
