/*
 * A261420  Numerators of coefficients in Taylor series expansion of sqrt( 1 +
 *          Sum_{k>=1} (k*(k+1)/2)*x^k ).
 * https://oeis.org/A261420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261420: Numerators of coefficients in the Taylor
   expansion of sqrt(1 + Σ_{k≥1} (k*(k+1)/2)*x^k).
   The denominators are powers of two.
   Recurrence:
       a(0) = 1
       for n ≥ 1:
           t_n = n*(n+1)/2
           M   = max_{1≤k≤n-1} (e_k + e_{n-k})      // exponents of previous terms
           numerator = t_n*2^M - Σ_{k=1}^{n-1} [ a_k*a_{n-k} * 2^{M-(e_k+e_{n-k})} ]
           let v2 = exponent of 2 dividing numerator
           a_n = numerator / 2^{M+1-v2}   // numerator is the returned value
                                           // denominator becomes 2^{e_n} where
                                           // e_n = M+1 - v2
*/

#include <stdlib.h>
#include <stdbool.h>

/* static cache for numerators and exponent of denominator */
static seq_t *num_cache   = NULL;   /* numerator of a(n)   */
static idx_t *exp_cache   = NULL;   /* exponent of 2 in a(n) denominator */
static size_t cache_size  = 0;      /* next index to be computed */

/* Compute a(n) and cache up to n */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp_num = (seq_t *)realloc(num_cache, new_size * sizeof(seq_t));
        if (!tmp_num) abort();
        num_cache = tmp_num;

        idx_t *tmp_exp = (idx_t *)realloc(exp_cache, new_size * sizeof(idx_t));
        if (!tmp_exp) abort();
        exp_cache = tmp_exp;

        /* initialise earliest entry if cache was empty */
        if (cache_size == 0) {
            num_cache[0] = (seq_t)1;
            exp_cache[0] = (idx_t)0;
            cache_size = 1;
        }

        /* compute new terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 1) {                    /* a(1) = 1/2 */
                num_cache[i] = (seq_t)1;
                exp_cache[i] = (idx_t)1;
                continue;
            }

            /* 1. compute t_i = i*(i+1)/2 */
            seq_t t_i = (seq_t)i * ((seq_t)i + (seq_t)1) / (seq_t)2;

            /* 2. find M = max_{1≤k<i} (exp_cache[k] + exp_cache[i-k]) */
            idx_t M = 0;
            for (idx_t k = 1; k < i; ++k) {
                idx_t e_sum = exp_cache[k] + exp_cache[i - k];
                if (e_sum > M) M = e_sum;
            }

            /* 3. RHS = t_i * 2^M */
            seq_t rhs = t_i << (int)M;

            /* 4. compute Σ_{k=1}^{i-1} a_k * a_{i-k} * 2^{M - (e_k+e_{i-k})} */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t prod = num_cache[k] * num_cache[i - k];
                idx_t shift = M - (exp_cache[k] + exp_cache[i - k]);  /* shift >= 0 */
                sum += prod << (int)shift;
            }

            /* 5. numerator before simplifying the power of 2 */
            seq_t numer = rhs - sum;

            /* 6. compute v2 = exponent of 2 dividing |numer| */
            idx_t v2 = 0;
            seq_t tmp = numer;
            if (tmp < (seq_t)0) tmp = -tmp;
            while ((tmp & (seq_t)1) == 0 && tmp != (seq_t)0) {
                tmp >>= 1;
                ++v2;
            }

            /* 7. reduce by 2^v2 */
            seq_t pow2 = (seq_t)1;
            pow2 <<= (int)v2;
            seq_t numer_final = numer / pow2;

            /* 8. exponent of denominator for a_i */
            idx_t e_i = M + 1 - v2;

            num_cache[i] = numer_final;
            exp_cache[i] = e_i;
        }

        cache_size = new_size;
    }

    return num_cache[n];
}
