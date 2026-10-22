/*
 * A318449  Numerators of the sequence whose Dirichlet convolution with itself
 *          yields A001511, the 2-adic valuation of 2n.
 * https://oeis.org/A318449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of f(n) where * f(1) = 1, * f(n) = (1/2) *
 *            (v2(2n) - Σ_{d|n, 1<d<n} f(d) * f(n/d)) * The code implements
 *            the definition by computing f(n) recursively and caching * the
 *            results. The sequence a(n) is the numerator of f(n) after
 *            reducing the * fraction to lowest terms. * * Note: v2(...
 * Range:     u8 n <= 2186, u16 n <= 9453, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/pow2.h"

/* Cached values of f(n) in numerator form and corresponding
   denominator exponent (i.e., f(n) = a[n] / 2⁽ᵉ[n]⁾). */
static seq_t  *a_cache  = NULL;
static idx_t  *e_cache  = NULL;
static idx_t   cache_max = 0;

/* Helper structure for temporary storage of {product, exponent sum}. */
typedef struct {
    seq_t  num;    /* a[d] * a[n/d] */
    idx_t  exp;    /* e[d] + e[n/d] */
} term_t;

/* Ensure that the cache contains entries up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Grow the arrays. */
    a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    e_cache = (idx_t *)realloc(e_cache, (n + 1) * sizeof(idx_t));

    /* Compute f(i) for all i > cache_max up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)1;
            e_cache[1] = 0;
            continue;
        }

        /*--- Gather all proper divisors d (1<d<i).---*/
        term_t  terms[256];
        int     tcnt = 0;
        idx_t   max_exp = 0;
        idx_t   limit = (idx_t)sqrt((double)i);

        for (idx_t d = 2; d <= limit; ++d) {
            if (i % d == 0) {
                idx_t d2 = i / d;
                /* First term: d */
                {
                    idx_t e1 = e_cache[d];
                    idx_t e2 = e_cache[d2];
                    idx_t exp_sum = e1 + e2;
                    seq_t  prod = a_cache[d] * a_cache[d2];
                    terms[tcnt].num = prod;
                    terms[tcnt].exp = exp_sum;
                    ++tcnt;
                    if (exp_sum > max_exp)
                        max_exp = exp_sum;
                }
                /* Second term: d2 (if distinct from d). */
                if (d2 != d) {
                    idx_t e1 = e_cache[d2];
                    idx_t e2 = e_cache[d];
                    idx_t exp_sum = e1 + e2;  /* same as previous */
                    seq_t prod = a_cache[d2] * a_cache[d];
                    terms[tcnt].num = prod;
                    terms[tcnt].exp = exp_sum;
                    ++tcnt;
                    if (exp_sum > max_exp)
                        max_exp = exp_sum;
                }
            }
        }

        /*--- Compute S_num (common denominator 2^max_exp).---*/
        seq_t S_num = (seq_t)0;
        for (int k = 0; k < tcnt; ++k) {
            idx_t shift = max_exp - terms[k].exp;
            seq_t term = terms[k].num;
            if (shift > 0)
                term = term * pow2(shift);
            S_num += term;
        }

        /*--- Compute g(i) = v2(2i) = 1 + v2(i).---*/
        idx_t v2 = 0;
        idx_t tmp = i;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++v2;
        }
        seq_t g_val  = (seq_t)1 + (seq_t)v2;
        seq_t g_term = g_val * pow2(max_exp);

        /*--- f(i) numerator before reduction.---*/
        seq_t numer = g_term - S_num;
        idx_t denom_exp = max_exp + 1;

        /* Reduce by powers of 2. */
        while ((numer & (seq_t)1) == 0) {
            numer >>= 1;
            --denom_exp;
        }

        a_cache[i] = numer;
        e_cache[i] = denom_exp;
    }

    cache_max = n;
}

/* -------------------------------------------------------------
 * Public API required by the build.
 * -------------------------------------------------------------
 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
