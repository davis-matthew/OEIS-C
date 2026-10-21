/*
 * A249700  Denominators of coefficients in series expansion of
 *          Cl_2(x)+x*log(x), where Cl_2 is the Clausen function of order 2.
 * https://oeis.org/A249700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of BernoulliB(n-1)/((n-1)*n!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Rational number in lowest terms (sign stored separately). */
typedef struct {
    seq_t num;     /* absolute value of numerator */
    seq_t den;     /* positive denominator */
    uint8_t neg;   /* 1 if negative, 0 otherwise */
} Rat;

/* Normalize a rational: ensure num and den coprime, den>0. */
static inline void rat_normalize(Rat *r)
{
    if (r->num == 0) {
        r->den = (seq_t)1;
        r->neg = 0;
        return;
    }
    seq_t g = gcd(r->num, r->den);
    r->num = r->num / g;
    r->den = r->den / g;
}

/* Add two rationals: res = a + b. */
static inline void rat_add(const Rat *a, const Rat *b, Rat *res)
{
    if (a->num == 0) { *res = *b; return; }
    if (b->num == 0) { *res = *a; return; }

    seq_t left  = a->num * b->den;
    seq_t right = b->num * a->den;
    res->den = a->den * b->den;

    if (a->neg == b->neg) {                /* same sign */
        res->neg = a->neg;
        res->num = left >= right ? left - right : right - left;
    } else {                                /* opposite signs */
        if (left >= right) {
            res->neg = a->neg;
            res->num = left - right;
        } else {
            res->neg = b->neg;
            res->num = right - left;
        }
    }

    if (res->num == 0) {
        res->den = (seq_t)1;
        res->neg = 0;
    } else {
        rat_normalize(res);
    }
}

/* Negate a rational: res = -a. */
static inline void rat_negate(const Rat *a, Rat *res)
{
    *res = *a;
    if (res->num != 0) res->neg ^= 1;
}

/* Array of Bernoulli numbers B[0..] stored as Rat. */
static Rat *B = NULL;          /* dynamic array */
static idx_t B_size = 0;       /* number of elements stored */

/* Ensure B[0..m] are computed.  Uses Akiyama–Tanigawa algorithm. */
static void ensure_bernoulli(idx_t m)
{
    if (B_size > m) return;  /* already available */

    size_t new_size = (size_t)(m + 1);
    B = (Rat *)realloc(B, new_size * sizeof(Rat));
    if (!B) exit(1);         /* allocation failed */

    for (idx_t i = B_size; i <= m; ++i) {
        if (i == 0) {         /* B0 = 1 */
            B[0].num = (seq_t)1;
            B[0].den = (seq_t)1;
            B[0].neg = 0;
            continue;
        }
        if (i == 1) {         /* B1 = -1/2 */
            B[1].num = (seq_t)1;
            B[1].den = (seq_t)2;
            B[1].neg = 1;
            continue;
        }
        if ((i & 1ULL) && i > 1) {   /* odd >1 => 0 */
            B[i].num = (seq_t)0;
            B[i].den = (seq_t)1;
            B[i].neg = 0;
            continue;
        }

        /* Compute B_i = - (1/(i+1)) * sum_{k=0}^{i-1} C(i+1,k) * B_k */
        Rat sum;
        sum.num = (seq_t)0;
        sum.den = (seq_t)1;
        sum.neg = 0;

        seq_t bin = (seq_t)1;    /* C(i+1,0) */

        for (idx_t k = 0; k < i; ++k) {
            if (B[k].num != 0) {
                /* term = bin * B_k */
                Rat term;
                term.neg = B[k].neg;
                term.num = B[k].num * bin;
                term.den = B[k].den;
                rat_normalize(&term);

                /* sum += term */
                Rat new_sum;
                rat_add(&sum, &term, &new_sum);
                sum = new_sum;
            }

            /* update bin to C(i+1, k+1) */
            seq_t mult  = (seq_t)(i + 1 - k);  /* (i+1-k) */
            seq_t denom = (seq_t)(k + 1);      /* (k+1) */
            bin = (bin * mult) / denom;       /* exact division */
        }

        /* B_i = -sum / (i+1) */
        Rat bm;
        rat_negate(&sum, &bm);               /* multiply by -1 */
        bm.den = bm.den * (seq_t)(i + 1);    /* divide by (i+1) */
        rat_normalize(&bm);
        B[i] = bm;
    }

    B_size = new_size;
}

/* Compute factorial of n (n!); n >= 0. */
static seq_t factorial(idx_t n)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        r = r * (seq_t)i;
    return r;
}

/* Public function: a(n) for OEIS sequence A249700. */
seq_t a(idx_t n)
{
    /* First three terms are defined as 1. */
    if (n < 3) return (seq_t)1;

    idx_t m = n - 1;   /* we need Bernoulli number B_m */
    ensure_bernoulli(m);

    const Rat *bm = &B[m];

    /* If B_m == 0 (odd m > 1), coefficient is zero ⇒ denominator 1. */
    if (bm->num == 0) return (seq_t)1;

    /* Abs value of numerator for gcd calculation. */
    seq_t Pabs = bm->num;

    /* Denominator part from B_m: B_m = num/den */
    seq_t denomB = bm->den;

    /* Compute (m+1)! = n! */
    seq_t fact = factorial((idx_t)(m + 1));

    /* Build d = denomB * m * (m+1)! */
    seq_t d = denomB * (seq_t)m;
    d = d * fact;

    seq_t g = gcd(Pabs, d);

    seq_t result = d / g;
    return result;
}
