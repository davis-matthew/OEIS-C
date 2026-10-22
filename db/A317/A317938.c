/*
 * A317938  Numerators of rational valued sequence whose Dirichlet convolution
 *          with itself yields sequence A001222 (bigomega n) + A063524 (1, 0,
 *          0, 0, ...).
 * https://oeis.org/A317938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of f(n), where f(1) = 1, f(n) = ½ *
 *            (bigomega(n) – Σ_{d|n, 1<d<n} f(d) * f(n/d)) (n > 1) The
 *            sequence a(n) is the integer numerator of f(n) after all factors
 *            of 2 in the denominator are cancelled
 * Range:     i8 n <= 15, i16 n <= 511, i32 n <= 9287, i64 n <= 9287,
 *            i128 n <= 9287, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#ifdef __cplusplus
#include <vector>
#endif

/* Cache of previously computed numerators and exponents of the
   denominator (f(n) = numerator / 2^exponent).  The cache is
   automatically expanded as needed. */
#ifdef __cplusplus
static std::vector<seq_t>   num_cache_vec;  /* numerator for index n */
static std::vector<idx_t>   exp_cache_vec;  /* exponent of 2 for index n */
#else
static seq_t*  num_cache = NULL;           /* numerator array */
static idx_t*  exp_cache = NULL;           /* exponent array */
#endif
static idx_t   cache_limit = 0;            /* largest n computed so far */

static void compute_up_to(idx_t n)
{
#ifdef __cplusplus
    /* Initialise the cache with a dummy entry at index 0. */
    if (num_cache_vec.empty()) {
        num_cache_vec.reserve(1);
        num_cache_vec.push_back(0);   /* unused */
        exp_cache_vec.push_back(0);   /* unused */
    }
    if (n <= cache_limit) return;

    for (idx_t k = cache_limit + 1; k <= n; ++k) {
        if (k == 1) {
            num_cache_vec.push_back(1);
            exp_cache_vec.push_back(0);
            continue;
        }

        seq_t  bigomega_k = bigomega(k);
        seq_t  sum_num = 0;        /* numerator of the sum   */
        idx_t  E = 0;              /* common denominator power of two for the sum */

        for (idx_t i = 2; i <= k / i; ++i) {
            if (k % i == 0) {
                idx_t d1 = i, d2 = k / i;
                seq_t num1 = num_cache_vec[d1];
                seq_t num2 = num_cache_vec[d2];
                idx_t  exp1 = exp_cache_vec[d1];
                idx_t  exp2 = exp_cache_vec[d2];

                seq_t prod_num = num1 * num2;
                idx_t prod_exp = exp1 + exp2;

                if (prod_exp > E) {
                    sum_num <<= (unsigned long long)(prod_exp - E);
                    E = prod_exp;
                }

                seq_t aligned_num = prod_num << (unsigned long long)(E - prod_exp);
                sum_num += aligned_num;
                if (d1 != d2)  /* complement divisor contributes the same product */
                    sum_num += aligned_num;
            }
        }

        seq_t fnum = (bigomega_k << (unsigned long long)E) - sum_num;
        idx_t fexp = E + 1;                 /* denominator exponent */

        /* Strip common factors of two from the numerator. */
        while ((fnum & 1) == 0) {
            fnum >>= 1;
            fexp--;
        }

        num_cache_vec.push_back(fnum);
        exp_cache_vec.push_back(fexp);
    }

    cache_limit = n;
#else
    if (n <= cache_limit) return;

    size_t new_cap = (size_t)n + 1;
    num_cache = realloc(num_cache, new_cap * sizeof(seq_t));
    exp_cache = realloc(exp_cache, new_cap * sizeof(idx_t));
    if (num_cache == NULL || exp_cache == NULL) {
        /* Allocation failed – abort.  In contest environments this
           should never happen for the intended limits. */
        abort();
    }

    for (idx_t k = cache_limit + 1; k <= n; ++k) {
        if (k == 1) {
            num_cache[1] = 1;
            exp_cache[1] = 0;
            continue;
        }

        seq_t  bigomega_k = bigomega(k);
        seq_t  sum_num = 0;        /* numerator of the sum   */
        idx_t  E = 0;              /* common denominator power of two for the sum */

        for (idx_t i = 2; i <= k / i; ++i) {
            if (k % i == 0) {
                idx_t d1 = i, d2 = k / i;
                seq_t num1 = num_cache[d1];
                seq_t num2 = num_cache[d2];
                idx_t  exp1 = exp_cache[d1];
                idx_t  exp2 = exp_cache[d2];

                seq_t prod_num = num1 * num2;
                idx_t prod_exp = exp1 + exp2;

                if (prod_exp > E) {
                    sum_num <<= (unsigned long long)(prod_exp - E);
                    E = prod_exp;
                }

                seq_t aligned_num = prod_num << (unsigned long long)(E - prod_exp);
                sum_num += aligned_num;
                if (d1 != d2)
                    sum_num += aligned_num;
            }
        }

        seq_t fnum = (bigomega_k << (unsigned long long)E) - sum_num;
        idx_t fexp = E + 1;

        while ((fnum & 1) == 0) {
            fnum >>= 1;
            fexp--;
        }

        num_cache[k] = fnum;
        exp_cache[k] = fexp;
    }

    cache_limit = n;
#endif
}

/* Public function: returns a(n). */
seq_t a(idx_t n)
{
    compute_up_to(n);
#ifdef __cplusplus
    return num_cache_vec[n];
#else
    return num_cache[n];
#endif
}
