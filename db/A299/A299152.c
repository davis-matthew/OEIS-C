/*
 * A299152  Denominators of the positive solution to 2^(n-1) = Sum_{d|n} a(d)
 *          * a(n/d).
 * https://oeis.org/A299152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator (power of two) of the rational solution
 *            to 2^(n-1) = Σ_{d|n} a(d)*a(n/d). The recurrence is a(n) =
 *            (2^(n-1) - Σ_{d|n, 1<d<n} a(d)*a(n/d)) / 2. The denominators are
 *            always powers of two
 * Range:     u8 n <= 1023, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Add a term term_num/2^term_exp to the accumulating sum
   sum_num/2^sum_exp, updating them in place. */
static inline void add_term(seq_t *sum_num, uint32_t *sum_exp,
                            seq_t term_num, uint32_t term_exp)
{
    if (term_exp > *sum_exp) {
        *sum_num <<= (term_exp - *sum_exp);
        *sum_exp   = term_exp;
    }
    *sum_num += term_num << (*sum_exp - term_exp);
}

/* Sequence a(n): denominator of the positive solution. */
seq_t a(idx_t n)
{
    /* Static storage for computed values. */
    static seq_t  *num   = NULL;   /* numerators a(k) * 2^exp[k] */
    static uint32_t *exps = NULL;  /* exponents: denom = 2^exps[k] */
    static size_t   cap   = 0;      /* allocated size          */
    static size_t   last  = 0;      /* highest n computed      */

    /* Grow storage if necessary. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t  *new_num  = (seq_t*)realloc(num,  new_cap * sizeof(*new_num));
        uint32_t *new_ex  = (uint32_t*)realloc(exps, new_cap * sizeof(*new_ex));
        if (!new_num || !new_ex) return (seq_t)0; /* allocation failed */
        num   = new_num;
        exps  = new_ex;
        cap   = new_cap;
    }

    /* Base case a(1)=1. */
    if (last == 0) {
        num[1]   = (seq_t)1;
        exps[1]  = 0;
        last     = 1;
    }

    /* Compute values up to n (inclusive). */
    for (size_t k = last + 1; k <= (size_t)n; ++k) {
        seq_t sum_num = 0;          /* numerator of the sum */
        uint32_t sum_exp = 0;       /* exponent of the denominator */

        /* Sum over proper divisors d, 1<d<k.  We add each term
           twice, because d and k/d both appear in the divisor list. */
        for (size_t d = 2; d * d <= k; ++d) {
            if (k % d != 0) continue;
            size_t nd = k / d;

            seq_t prod_num = num[d] * num[nd];
            uint32_t prod_exp = exps[d] + exps[nd];

            /* Add the term for (d, nd). */
            add_term(&sum_num, &sum_exp, prod_num, prod_exp);

            /* If nd != d, add the symmetric term. */
            if (nd != d) {
                add_term(&sum_num, &sum_exp, prod_num, prod_exp);
            }
        }

        /* RHS is 2^(k-1).  Bring it to the same denominator 2^sum_exp. */
        seq_t rhs_num = pow2((idx_t)(k - 1 + sum_exp));

        /* Compute numerator of the difference: rhs - sum. */
        seq_t diff_num = rhs_num - sum_num;
        uint32_t diff_exp = sum_exp;

        /* a(k) = (rhs - sum) / 2. */
        seq_t anum = diff_num;
        uint32_t aexp = diff_exp + 1;

        /* Remove common factors of 2 from numerator and exponent. */
        while (aexp > 0 && ((anum & (seq_t)1) == 0)) {
            anum >>= 1;
            --aexp;
        }

        /* Store computed value. */
        num[k]   = anum;
        exps[k]  = aexp;
    }

    last = (size_t)n;

    /* Denominator is 2^exps[n]. */
    return pow2((idx_t)exps[n]);
}
