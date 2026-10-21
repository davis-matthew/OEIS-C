/*
 * A274653  Numerators of coefficients of z^n/n! for the expansion of Fricke's
 *          hypergeometric function F_1(1/2,1/2;z).
 * https://oeis.org/A274653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of r(n) defined by r(0) = 0, r(n) =
 *            ((2*n-1)^2/(4*n)) * r(n-1) + 2*c(n)/(n*(2*n-1)), n >= 1, where
 *            c(n) = ((2*n)!)^2 / (n!^3 * 2^(4*n)) (a rational number)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *num = NULL;      /* numerator of r(i) */
    static seq_t *den = NULL;      /* denominator of r(i) */
    static seq_t *c_num = NULL;    /* numerator of c(i) */
    static seq_t *c_den = NULL;    /* denominator of c(i) */
    static idx_t max_cached = 0;   /* highest n computed so far */

    /* initialisation on first call */
    if (!num) {
        num    = (seq_t *)malloc(sizeof(seq_t));
        den    = (seq_t *)malloc(sizeof(seq_t));
        c_num  = (seq_t *)malloc(sizeof(seq_t));
        c_den  = (seq_t *)malloc(sizeof(seq_t));
        if (!num || !den || !c_num || !c_den) return 0;  /* error fallback */
        num[0]   = 0;
        den[0]   = 1;
        c_num[0] = 1;   /* to obtain c(1)=1/4 via recurrence */
        c_den[0] = 1;
        max_cached = 0;
    }

    if (n == 0) return num[0];

    if (n > max_cached) {
        idx_t newmax = n;
        num   = (seq_t *)realloc(num,   (newmax + 1) * sizeof(seq_t));
        den   = (seq_t *)realloc(den,   (newmax + 1) * sizeof(seq_t));
        c_num = (seq_t *)realloc(c_num, (newmax + 1) * sizeof(seq_t));
        c_den = (seq_t *)realloc(c_den, (newmax + 1) * sizeof(seq_t));
        if (!num || !den || !c_num || !c_den) return 0;  /* error fallback */

        for (idx_t i = max_cached + 1; i <= newmax; ++i) {
            /* (2*i-1) and its square */
            seq_t two_i_minus1 = (seq_t)(2 * i - 1);
            seq_t factor = two_i_minus1 * two_i_minus1;   /* (2*i-1)^2 */

            /* Compute c(i) = c(i-1) * factor / (4*i)   (reduced) */
            seq_t num_c = c_num[i - 1] * factor;
            seq_t den_c = c_den[i - 1] * (seq_t)(4 * i);
            seq_t g_c = gcd(num_c, den_c);
            num_c /= g_c;
            den_c /= g_c;
            c_num[i] = num_c;
            c_den[i] = den_c;

            /* Term 1: q * r(i-1) where q = factor / (4*i) */
            seq_t num1 = num[i - 1] * factor;
            seq_t den1 = den[i - 1] * (seq_t)(4 * i);
            seq_t g1 = gcd(num1, den1);
            num1 /= g1;
            den1 /= g1;

            /* Term 2: 2*c(i) / (i*(2*i-1)) */
            seq_t num2 = c_num[i] * 2;
            seq_t den2 = c_den[i] * (seq_t)i * two_i_minus1;
            seq_t g2 = gcd(num2, den2);
            num2 /= g2;
            den2 /= g2;

            /* Add the two fractions */
            seq_t g_common = gcd(den1, den2);
            seq_t den_sum = den1 * (den2 / g_common);
            seq_t num_sum = num1 * (den2 / g_common) + num2 * (den1 / g_common);
            seq_t g_sum = gcd(num_sum, den_sum);
            num_sum /= g_sum;
            den_sum /= g_sum;

            num[i] = num_sum;
            den[i] = den_sum;
        }
        max_cached = newmax;
    }
    return num[n];
}
