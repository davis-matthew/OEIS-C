/*
 * A318454  Denominators of the sequence whose Dirichlet convolution with
 *          itself yields A001227, number of odd divisors of n.
 * https://oeis.org/A318454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of f(n) where f(1)=1 and for n>1
 *            f(n)=(1/2)*(A001227(n)-Σ_{d|n,d>1,d<n} f(d)*f(n/d)). The
 *            sequence is multiplicative and only powers of two appear as
 *            denominators. The value a(n) is 1 if n is odd, otherwise it
 *            equals 2^k where k is the exponent of the denominator of f(2^v),
 *            v=v...
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* pow2(seq_t exp) */

typedef struct {
    seq_t num;   /* odd numerator of f(2^e) */
    idx_t k;     /* denominator exponent of f(2^e), i.e. f = num/2^k */
} data_t;

static data_t *h_frac = NULL;   /* array of f(2^e) fractions   */
static idx_t *den_exp = NULL;   /* array of denominator exponents a(2^e) */
static idx_t h_cap = 0;
static idx_t max_e_computed = 0;

/* double capacity as needed */
static void grow_arrays(idx_t need_cap) {
    if (need_cap <= h_cap) return;
    idx_t new_cap = h_cap ? h_cap * 2 : 16;
    if (new_cap < need_cap) new_cap = need_cap;
    data_t *new_h = (data_t *)realloc(h_frac, new_cap * sizeof(data_t));
    h_frac = new_h;
    h_cap = new_cap;
    idx_t *new_den = (idx_t *)realloc(den_exp, new_cap * sizeof(idx_t));
    den_exp = new_den;
}

/* compute f(2^e) and a(2^e) for all e up to 'e' */
static void ensure_pow2(idx_t e) {
    if (e <= max_e_computed) return;
    grow_arrays(e + 1);
    /* compute from previous values */
    for (idx_t i = max_e_computed + 1; i <= e; ++i) {
        /* compute S = Σ_{j=1}^{i-1} h[j]*h[i-j] */
        seq_t sum_num = 0;
        idx_t sum_exp = 0;
        for (idx_t j = 1; j < i; ++j) {
            seq_t prod_num = h_frac[j].num * h_frac[i - j].num;
            idx_t prod_exp = h_frac[j].k + h_frac[i - j].k;
            if (sum_num == 0) {
                sum_num = prod_num;
                sum_exp = prod_exp;
            } else {
                if (sum_exp <= prod_exp) {
                    sum_num <<= (prod_exp - sum_exp);  /* multiply by 2^{diff} */
                    sum_num += prod_num;
                    sum_exp = prod_exp;
                } else {
                    prod_num <<= (sum_exp - prod_exp);
                    sum_num += prod_num;
                    /* sum_exp unchanged */
                }
            }
        }

        /* numerator for f(2^i): 1 - S = (2^{sum_exp} - sum_num) / 2^{sum_exp} */
        seq_t pow_sum = pow2(sum_exp);            /* 2^{sum_exp} */
        seq_t num_tmp = pow_sum - sum_num;        /* 2^{sum_exp} - sum_num */
        idx_t denom = sum_exp + 1;                /* division by 2 */

        /* reduce common factor 2 */
        idx_t tz = 0;
        while ((num_tmp & 1) == 0 && tz < denom) {
            num_tmp >>= 1;
            tz++;
            denom--;
        }

        h_frac[i].num = num_tmp;
        h_frac[i].k   = denom;
        den_exp[i]    = denom;
    }
    max_e_computed = e;
}

/* return the sequence value a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* find exponent of 2 in n */
    idx_t v = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        ++v;
        m >>= 1;
    }
    if (v == 0) return 1;           /* odd n -> denominator 1 */

    ensure_pow2(v);
    idx_t exp = den_exp[v];
    return pow2(exp);               /* power of two */
}
