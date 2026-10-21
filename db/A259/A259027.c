/*
 * A259027  a(n) is the numerator of c(n), where c(n) is calculated from
 *          Product_{i>=1}(1-c(i)*x^i) = exp(-(x^2)/(1-x))*(1-x).
 * https://oeis.org/A259027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of c(n), where c(1)=1 and for n>1 Σ_{d|n} (1/d)
 *            * c(n/d)^d = 1 + 1/n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the numerator of c(n).  It is memoised so that
   calls in any order are handled efficiently.  No global state other
   than the cached arrays is used.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* External types supplied by the build.  */
typedef uint64_t idx_t;   /* index type */
            /* seq_t is defined by the build; no typedef here */

/* Cached values of c(n) as numerators and denominators.  */
static seq_t *c_num = NULL;
static seq_t *c_den = NULL;
static size_t cap = 0;          /* allocated size (≥0) */
static idx_t used = 0;          /* highest index computed (≥0) */

/* Ensure capacity for indices 0 … n (index n may be accessed).  */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cap)
        return;
    size_t new_cap = (cap == 0) ? 2 : cap << 1;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;
    seq_t *new_num = (seq_t *)realloc(c_num, new_cap * sizeof(seq_t));
    seq_t *new_den = (seq_t *)realloc(c_den, new_cap * sizeof(seq_t));
    if (!new_num || !new_den)
        abort();   /* out of memory */
    for (size_t i = cap; i < new_cap; ++i) {
        new_num[i] = (seq_t)0;
        new_den[i] = (seq_t)1;
    }
    c_num = new_num;
    c_den = new_den;
    cap = new_cap;
}

/* Compute c(k) for all k ≤ n, storing numerators and denominators. */
static void compute_up_to(idx_t n)
{
    if (n <= used)
        return;

    if (used == 0) {   /* initialise c(1)=1 */
        ensure_capacity(1);
        c_num[1] = (seq_t)1;
        c_den[1] = (seq_t)1;
        used = 1;
    }

    for (idx_t k = used + 1; k <= n; ++k) {
        /* sum of terms for divisors d > 1 (the term for d=1 will be isolated) */
        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;

        /* divisor d = k : term = 1/k  (since c(1)=1) */
        {
            seq_t term_num = (seq_t)1;
            seq_t term_den = (seq_t)k;
            seq_t new_num = sum_num * term_den + term_num * sum_den;
            seq_t new_den = sum_den * term_den;
            seq_t g = gcd(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;
        }

        /* loop over remaining divisors 2 … sqrt(k) */
        for (idx_t d = 2; d * d <= k; ++d) {
            if (k % d != 0)
                continue;

            /* divisor d */
            idx_t m = k / d;                  /* m = k/d */
            seq_t p = c_num[m];
            seq_t q = c_den[m];
            seq_t p_pow = big_pow(p, (seq_t)d);
            seq_t q_pow = big_pow(q, (seq_t)d);
            seq_t term_num = p_pow;
            seq_t term_den = seq_mul(q_pow, (seq_t)d);

            seq_t new_num = sum_num * term_den + term_num * sum_den;
            seq_t new_den = sum_den * term_den;
            seq_t g = gcd(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;

            /* paired divisor d2 = k/d  (if different and >1) */
            idx_t d2 = k / d;
            if (d2 != d && d2 > 1) {
                idx_t m2 = k / d2;            /* m2 = k/d2 = d */
                seq_t p2 = c_num[m2];
                seq_t q2 = c_den[m2];
                seq_t p2_pow = big_pow(p2, (seq_t)d2);
                seq_t q2_pow = big_pow(q2, (seq_t)d2);
                seq_t term_num2 = p2_pow;
                seq_t term_den2 = seq_mul(q2_pow, (seq_t)d2);

                new_num = sum_num * term_den2 + term_num2 * sum_den;
                new_den = sum_den * term_den2;
                g = gcd(new_num, new_den);
                sum_num = new_num / g;
                sum_den = new_den / g;
            }
        }

        /* target value = 1 + 1/k  as fraction */
        seq_t target_num = (seq_t)(k + 1);
        seq_t target_den = (seq_t)k;

        /* c(k) = target - sum */
        seq_t diff_num = target_num * sum_den - target_den * sum_num;
        seq_t diff_den = target_den * sum_den;
        seq_t g = gcd(diff_num, diff_den);
        diff_num = diff_num / g;
        diff_den = diff_den / g;

        c_num[k] = diff_num;
        c_den[k] = diff_den;
    }
    used = n;
}

/* Public function required by the build.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */
    ensure_capacity(n);
    compute_up_to(n);
    return c_num[n];
}
