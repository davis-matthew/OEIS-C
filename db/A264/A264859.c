/*
 * A264859  a(n) is the denominator of c(n), where c(n) is calculated from
 *          Product_{i>=1}(1-c(i)*x^i) = exp(-(x^2)/(1-x))*(1-x).
 * https://oeis.org/A264859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of c(n), where c(1)=1 and for n>1 * c(n)
 *            satisfies Sum_{d|n, d>1} (1/d)*c(n/d)^d = 1. * Thus c(n) = 1 -
 *            sum_{d|n, 1<d<n} (1/d)*c(n/d)^d. * The sequence a(n) is the
 *            denominator of the reduced fraction c(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"  /* gcd(seq_t a, seq_t b) */

static seq_t *cnum = NULL;          /* numerators c(n) */
static seq_t *cden = NULL;          /* denominators c(n) */
static size_t cache_cap = 0;        /* capacity of the arrays   */
static idx_t computed_up_to = 0;    /* last n for which values are known */

/* ensure the cache is large enough */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
        while (new_cap <= (size_t)n) new_cap *= 2;
        cnum = (seq_t *)realloc(cnum, new_cap * sizeof(seq_t));
        cden = (seq_t *)realloc(cden, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* fast exponentiation for seq_t with idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* compute c(k) for all k up to n */
static void compute_upto(idx_t n)
{
    if (computed_up_to >= n) return;
    ensure_cache(n);

    if (computed_up_to == 0) {          /* initialise c(1) */
        cnum[1] = (seq_t)1;
        cden[1] = (seq_t)1;
        computed_up_to = 1;
    }

    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum_num = (seq_t)0;      /* represents 0/1 initially */
        seq_t sum_den = (seq_t)1;

        /* iterate over proper divisors d of i, with 1 < d < i */
        for (idx_t d = 2; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor d, base index m = i/d */
                idx_t m = i / d;
                seq_t pnum = pow_seq(cnum[m], d);
                seq_t pden = pow_seq(cden[m], d);
                pden = pden * (seq_t)d;   /* multiply by divisor d */

                /* add term pnum/pden to the sum */
                seq_t tmp_num = sum_num * pden + pnum * sum_den;
                seq_t tmp_den = sum_den * pden;
                seq_t g = gcd(tmp_num, tmp_den);
                sum_num = tmp_num / g;
                sum_den = tmp_den / g;

                /* second divisor e = i/d, if distinct from d */
                idx_t e = m;
                if (e != d) {
                    seq_t pnum2 = pow_seq(cnum[d], e);
                    seq_t pden2 = pow_seq(cden[d], e);
                    pden2 = pden2 * (seq_t)e;

                    seq_t tmp_num2 = sum_num * pden2 + pnum2 * sum_den;
                    seq_t tmp_den2 = sum_den * pden2;
                    seq_t g2 = gcd(tmp_num2, tmp_den2);
                    sum_num = tmp_num2 / g2;
                    sum_den = tmp_den2 / g2;
                }
            }
        }

        /* c(i) = 1 - sum => (sum_den - sum_num) / sum_den */
        seq_t c_num = sum_den - sum_num;
        seq_t c_den = sum_den;
        seq_t g = gcd(c_num, c_den);
        c_num = c_num / g;
        c_den = c_den / g;

        cnum[i] = c_num;
        cden[i] = c_den;
    }

    computed_up_to = n;
}

/* public function requested by the exercise */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cden[n];
}
