/*
 * A290159  Numerators of coefficients in Taylor series expansion of
 *          (1+x+x^2)^(1/2).
 * https://oeis.org/A290159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   * a(0) = 1 * a(1) = 1 * for n ≥ 2 * a(n) = -(1/2) *
 *            Σ_{k=1}^{n-1} a(k) * a(n-k) * (after cancelling common factors
 *            of 2, the returned value is the * numerator of the simplified
 *            rational coefficient of x^n in * (1+x+x²)^(1/2).)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of already computed terms */
    static seq_t *num = NULL;       /* numerator of a(i) */
    static uint64_t *expv = NULL;   /* exponent of 2 in denominator of a(i) */
    static idx_t cached_n = 0;      /* largest n computed so far */
    static size_t capacity = 0;     /* allocated size of the cache */

    /* Ensure capacity for all indices up to n */
    if ((size_t)n + 1 > capacity) {
        capacity = (size_t)n + 1;
        num  = (seq_t *)realloc(num,  capacity * sizeof(seq_t));
        expv = (uint64_t *)realloc(expv, capacity * sizeof(uint64_t));
    }

    /* Base cases */
    if (n == 0) {
        if (capacity < 1) {           /* first time we need index 0 */
            capacity = 1;
            num  = (seq_t *)realloc(num,  capacity * sizeof(seq_t));
            expv = (uint64_t *)realloc(expv, capacity * sizeof(uint64_t));
        }
        num[0]  = (seq_t)1;
        expv[0] = 0;
        cached_n = 0;
        return num[0];
    }

    /* Pre‑compute terms up to n if needed */
    if (n > cached_n) {
        /* Compute missing terms in increasing order */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1) {                 /* a(1) = 1/2 => numerator 1, exp 1 */
                num[i]  = (seq_t)1;
                expv[i] = 1;
                continue;
            }

            /* Sum over k = 1 … i-1 of a(k) * a(i-k) */
            seq_t sum_num = (seq_t)0;
            uint64_t max_exp = 0;

            for (idx_t k = 1; k < i; ++k) {
                idx_t j = i - k;
                seq_t prod_num = num[k] * num[j];
                uint64_t prod_exp = expv[k] + expv[j];

                /* If product exp is larger than current max, shift the
                 * accumulated sum to match the new exponent.
                 */
                if (prod_exp > max_exp) {
                    sum_num = sum_num * ((seq_t)1 << (size_t)(prod_exp - max_exp));
                    max_exp = prod_exp;
                }

                /* Bring the product to the common exponent and add it. */
                uint64_t shift = max_exp - prod_exp;
                prod_num = prod_num * ((seq_t)1 << (size_t)shift);
                sum_num += prod_num;
            }

            seq_t result_num;
            uint64_t exp_res;

            if (i < 3) { /* R(n)=1 for n=1,2 */
                seq_t r_scaled = ((seq_t)1 << (size_t)max_exp);
                result_num = r_scaled - sum_num;
            } else {     /* R(n)=0 for n>=3 */
                result_num = -sum_num;
            }
            exp_res = max_exp + 1;           /* multiply by 1/2 */

            /* Reduce the fraction by removing common powers of 2. */
            while (exp_res > 0 && result_num % (seq_t)2 == 0) {
                result_num = result_num / (seq_t)2;
                exp_res--;
            }

            num[i]  = result_num;
            expv[i] = exp_res;
        }
        cached_n = n;
    }

    return num[n];
}
