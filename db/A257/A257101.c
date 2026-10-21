/*
 * A257101  From fifth root of the inverse of Riemann zeta function: form
 *          Dirichlet series Sum b(n)/n^x whose fifth power is 1/zeta;
 *          sequence gives numerator of b(n).
 * https://oeis.org/A257101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of ∏_{p^e∥n} ( (-1)^e * (1/5 choose e) ), *
 *            where (1/5 choose e) = (1/5)(1/5-1)…(1/5-e+1)/e!
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Smallest prime factor table (dynamic, expanded on demand). */
static uint64_t *spf = NULL;
static size_t spf_size = 0;

/* Expand the SPF table up to `limit`. */
static void build_spf(size_t limit)
{
    if (limit <= spf_size) return;
    uint64_t *new_spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* Cache of reduced coefficients for binom(1/5, e). */
static seq_t coef_num[65];
static seq_t coef_den[65];
static int coef_max_computed = 0;

/* Compute coefficients up to exponent `emax`. */
static void compute_coef_up_to(size_t emax)
{
    if (emax <= (size_t)coef_max_computed) return;
    for (int e = coef_max_computed + 1; e <= (int)emax; ++e) {
        seq_t num = 1;
        seq_t den = 1;
        for (int t = 1; t <= e; ++t) {
            int mult_int = 1 - 5 * (t - 1);
            seq_t mult = (seq_t)mult_int;
            num = num * mult;
            den = den * ((seq_t)5 * (seq_t)t);
        }
        if (e & 1) num = -num;          /* apply the (-1)^e sign */
        /* Reduce by GCD (use absolute values). */
        seq_t abs_num = num >= 0 ? num : -num;
        seq_t abs_den = den >= 0 ? den : -den;
        seq_t g = gcd(abs_num, abs_den);
        if (g > 1) {
            num /= g;
            den /= g;
        }
        coef_num[e] = num;
        coef_den[e] = den;
    }
    coef_max_computed = (int)emax;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, harmless */
    build_spf((size_t)n);                /*ensure SPF up to n*/

    seq_t num = 1;                       /* running numerator   */
    seq_t den = 1;                       /* running denominator */

    idx_t m = n;
    while (m > 1) {
        uint64_t p = spf[(size_t)m];
        size_t e = 0;
        while (m % p == 0) { m /= p; ++e; }

        if (e > (size_t)coef_max_computed) compute_coef_up_to(e);

        num *= coef_num[e];
        den *= coef_den[e];

        /* Reduce using absolute values to avoid negative GCD. */
        seq_t abs_num = num >= 0 ? num : -num;
        seq_t abs_den = den >= 0 ? den : -den;
        seq_t g = gcd(abs_num, abs_den);
        if (g > 1) {
            num /= g;
            den /= g;
        }
    }
    return num;                           /* return the reduced numerator */
}
