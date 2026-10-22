/*
 * A335264  a(n) = Numerator(-4*n^2*Zeta(1 - n)^2*(1 - 2^n)) for n >= 1, a(0)
 *          = 0.
 * https://oeis.org/A335264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(Bernoulli(n)^2*(2^(n+2)-4)) for n >= 0
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 11, u64 n <= 23, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0; for odd n > 1 the result is 0. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

typedef struct {
    seq_t num;   /* absolute value of numerator (always >= 0) */
    seq_t den;   /* positive denominator (always > 0) */
    int   sign;  /* +1 or -1, used only in intermediate computations */
} frac_t;

/* ------------------------------------------------------------------ */
/*  GCD for seq_t ---------------------------------------------------- */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Reduce a fraction to lowest terms -------------------------------- */
static void normalize(frac_t *f)
{
    if (f->num == (seq_t)0) {
        f->den   = (seq_t)1;
        f->sign  = 1;
        return;
    }
    seq_t g = gcd_seq(f->num, f->den);
    if (g > (seq_t)1) {
        f->num  /= g;
        f->den  /= g;
    }
    /* Ensure denominator is positive */
    if ((long long)f->den < 0) {
        f->den  = -f->den;
        f->sign = -f->sign;
    }
    if (f->num == (seq_t)0)      /* sign irrelevant for zero */
        f->sign = 1;
}

/* ------------------------------------------------------------------ */
/*  Add a signed fraction (multiplier * frac) to a sum ---------------- */
static void add_frac_assign(frac_t *sum, const frac_t *add, int mult)
{
    int add_sign = add->sign * mult;      /* +1 or -1 */

    /* Compute numerator of the new fraction */
    seq_t prod1   = sum->num * add->den;   /* sum.num * add.den   */
    seq_t prod2   = add->num * sum->den;   /* add.num * sum.den   */

    /* Add or subtract the numerators depending on signs */
    if (sum->sign == add_sign) {          /* same sign */
        sum->num  = prod1 + prod2;
    } else {                              /* opposite sign */
        if (prod1 >= prod2) {
            sum->num = prod1 - prod2;
        } else {
            sum->num = prod2 - prod1;
        }
        sum->sign = add_sign;
    }

    /* LCM of denominators is product because both denominators are
       positive; we keep the product and reduce later */
    sum->den  = sum->den * add->den;
    normalize(sum);
}

/* ------------------------------------------------------------------ */
/*  Bernoulli numbers cache ---------------------------------------- */
static frac_t *B_cache    = NULL;
static idx_t   B_size     = 0;
static idx_t   B_computed = 0;

/* Compute Bernoulli numbers up to n (inclusive) */
static void compute_B_upto(idx_t n)
{
    if (n <= B_computed)
        return;

    /* Expand cache if necessary */
    if (B_size < n + 1) {
        idx_t new_size = n + 1;
        frac_t *tmp = (frac_t *)realloc(B_cache, new_size * sizeof(frac_t));
        if (!tmp && new_size > 0)
            exit(EXIT_FAILURE);
        B_cache = tmp;
        B_size  = new_size;
    }

    /* Initialise new entries */
    for (idx_t i = B_computed + 1; i <= n; ++i) {
        B_cache[i].num  = (seq_t)0;
        B_cache[i].den  = (seq_t)1;
        B_cache[i].sign = 1;
    }

    /* Base cases */
    if (B_computed == 0) {
        B_cache[0].num  = (seq_t)1;
        B_cache[0].den  = (seq_t)1;
        B_cache[0].sign = 1;
    }
    if (n >= 1 && B_computed < 1) {
        B_cache[1].num  = (seq_t)1;
        B_cache[1].den  = (seq_t)2;
        B_cache[1].sign = -1;      /* B1 = -1/2 */
    }

    /* Compute from B_computed+1 up to n using the Akiyama–Tanigawa recurrence */
    for (idx_t m = B_computed + 1; m <= n; ++m) {
        /* B_m is zero for odd m > 1 */
        if ((m & 1) && m != 1) {
            B_cache[m].num  = (seq_t)0;
            B_cache[m].den  = (seq_t)1;
            B_cache[m].sign = 1;
            continue;
        }

        /* Compute S = sum_{k=0}^{m-1} binom(m+1, k) * B_k */
        frac_t sum;
        sum.num  = (seq_t)0;
        sum.den  = (seq_t)1;
        sum.sign = 1;   /* zero */

        for (idx_t k = 0; k < m; ++k) {
            if (B_cache[k].num == (seq_t)0)
                continue;
            frac_t term = B_cache[k];
            seq_t  comb = binomial(m + 1, k);
            term.num    = term.num * comb;   /* multiply numerator by binomial coefficient */
            add_frac_assign(&sum, &term, +1);
        }

        /* B_m = - S / (m+1) */
        if (sum.num == (seq_t)0) {
            B_cache[m].num  = (seq_t)0;
            B_cache[m].den  = (seq_t)1;
            B_cache[m].sign = 1;
        } else {
            B_cache[m].num  = sum.num;
            B_cache[m].den  = sum.den * (m + 1);
            B_cache[m].sign = -sum.sign;
            normalize(&B_cache[m]);
        }
    }

    B_computed = n;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute Bernoulli number B_n */
    compute_B_upto(n);

    /* If B_n is zero (odd n > 1) return 0 */
    if (B_cache[n].num == (seq_t)0)
        return (seq_t)0;

    /* factor = 2^(n+2) - 4  (positive for n >= 1) */
    seq_t factor = pow2(n + 2) - (seq_t)4;

    /* numerator = B_num^2 * factor */
    seq_t num = B_cache[n].num * B_cache[n].num;
    num = num * factor;

    /* denominator = B_den^2 */
    seq_t den = B_cache[n].den * B_cache[n].den;

    /* Reduce the fraction and return the numerator */
    seq_t g = gcd_seq(num, den);
    return num / g;
}
