/*
 * A318650  Numerators of the sequence whose Dirichlet convolution with itself
 *          yields A057521, the powerful part of n.
 * https://oeis.org/A318650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = numerator of f(n), where f(1) = 1, for n > 1: f(n) =
 *            (1/2) * (A057521(n) - Σ_{d|n, 1<d<n} f(d) * f(n/d)) Here
 *            A057521(n) is the powerful part of n. The function memoises the
 *            values and returns a(n) for any n ≥ 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8191, u64 n <= 9727,
 *            u128 n <= 9727, big any n
 * Helpers:   factor_u64, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/factor_u64.h"      /* factor_u64 */
#include "../utils/int_sqrt.h"        /* int_sqrt */

static seq_t  *num  = NULL;            /* numerator of f(n)      */
static unsigned char *den = NULL;      /* 2‑exponent of denominator */
static idx_t max_mem = 0;              /* highest n computed so far */

/* Compute f for all n in (max_mem, limit] */
static void compute_to(idx_t limit)
{
    size_t new_cap = limit + 1;   /* 1‑based arrays */
    if (!num) {
        num  = (seq_t *)malloc(new_cap * sizeof(seq_t));
        den  = (unsigned char *)malloc(new_cap * sizeof(unsigned char));
        num[1] = 1;
        den[1] = 0;
        max_mem = 1;
    } else if (limit > max_mem) {
        num  = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
        den  = (unsigned char *)realloc(den, new_cap * sizeof(unsigned char));
    }

    for (idx_t n = max_mem + 1; n <= limit; ++n) {
        /* powerful part of n */
        uint64_t primes[64], exps[64];
        size_t cnt = 0;
        factor_u64((uint64_t)n, primes, exps, &cnt);

        seq_t powerful = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] >= 2) {
                seq_t p_pow = 1;
                for (uint64_t k = 0; k < exps[i]; ++k)
                    p_pow *= (seq_t)primes[i];
                powerful *= p_pow;
            }
        }

        /* Σ f(d)*f(n/d) over proper divisors d>1,d<n */
        seq_t sum_num = 0;
        unsigned int sum_exp = 0;   /* denominator exponent for the sum */

        idx_t root = int_sqrt((idx_t)n);
        for (idx_t d = 2; d <= root; ++d) {
            if (n % d) continue;
            idx_t e = n / d;
            seq_t prod_num = num[d] * num[e];
            unsigned int prod_exp = den[d] + den[e];

            /* add for d */
            if (sum_exp < prod_exp) {
                sum_num <<= (prod_exp - sum_exp);
                sum_exp = prod_exp;
                sum_num += prod_num;
            } else if (sum_exp > prod_exp) {
                sum_num += prod_num << (sum_exp - prod_exp);
            } else {
                sum_num += prod_num;
            }

            /* add for e when distinct */
            if (d != e) {
                if (sum_exp < prod_exp) {
                    sum_num <<= (prod_exp - sum_exp);
                    sum_exp = prod_exp;
                    sum_num += prod_num;
                } else if (sum_exp > prod_exp) {
                    sum_num += prod_num << (sum_exp - prod_exp);
                } else {
                    sum_num += prod_num;
                }
            }
        }

        /* f(n) = (powerful*2^sum_exp - sum_num) / (2*2^sum_exp) */
        seq_t numerator = powerful << sum_exp;
        numerator -= sum_num;
        unsigned int denom_exp = sum_exp + 1;   /* outer factor 1/2 */

        /* reduce common factors of 2 */
        while ((numerator & 1) == 0 && denom_exp > 0) {
            numerator >>= 1;
            denom_exp--;
        }

        num[n] = numerator;
        den[n] = (unsigned char)denom_exp;
    }

    max_mem = limit;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < 1) n = 1;         /* safety */
    if (n > max_mem) compute_to(n);
    return num[n];
}
