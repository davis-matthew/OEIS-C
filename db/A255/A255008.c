/*
 * A255008  Array T(n,k) read by ascending antidiagonals, where T(n,k) is the
 *          numerator of polygamma(n, 1) - polygamma(n, k).
 * https://oeis.org/A255008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 31, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   seq_gcd, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A255008
    a(n) is the numerator of the reduced fraction
    T(n',k') = (-1)^(n'+1)*n'! * Σ_{j=1}^{k'-1} 1/j^(n'+1)
    where the pair (n',k') is obtained by reading the array T by
    ascending antidiagonals:  (0,1),(1,1),(0,2),(2,1),(1,2),(0,3),…
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/seq_realloc.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

/* factorial cache */
static seq_t *fac = NULL;
static idx_t fac_cap = 0;   /* current capacity of fac array */

/* ensure that fac[n] is computed for all n <= need */
static void ensure_fac(idx_t need)
{
    if (fac_cap <= need) {
        idx_t new_cap = fac_cap == 0 ? 1 : fac_cap;
        while (new_cap <= need) new_cap <<= 1;
        idx_t old_cap = fac_cap;
        fac = seq_realloc(fac, fac_cap, new_cap);
        if (old_cap == 0) {          /* first allocation */
            fac[0] = (seq_t)1;
            old_cap = 1;
        }
        for (idx_t i = old_cap; i < new_cap; ++i)
            fac[i] = fac[i-1] * (seq_t)i;
        fac_cap = new_cap;
    }
}

/* compute numerator of T(n,k) with n>=0, k>=1 */
static seq_t T_numerator(idx_t n, idx_t k)
{
    if (k == 1) return (seq_t)0;   /* harmonic sum is zero */

    /* harmonic number of order n+1 */
    seq_t H_num = 0;          /* numerator */
    seq_t H_den = 1;          /* denominator */

    idx_t exp = n + 1;

    for (idx_t j = 1; j < k; ++j) {
        seq_t j_pow = seq_pow((seq_t)j, exp);
        seq_t new_num = H_num * j_pow + H_den;   /* H_num/j_pow + 1/j_pow */
        seq_t new_den = H_den * j_pow;
        seq_t g = seq_gcd(new_num, new_den);
        H_num = new_num / g;
        H_den = new_den / g;
    }

    ensure_fac(n);
    seq_t fact = fac[n];

    /* cancel common factor between factorial and denominator */
    seq_t g2 = seq_gcd(fact, H_den);
    seq_t fact_div = fact / g2;
    seq_t den_tmp  = H_den / g2;

    seq_t num_tmp = H_num * fact_div;

    /* final reduction */
    seq_t g3 = seq_gcd(num_tmp, den_tmp);
    seq_t final_num = num_tmp / g3;
    /* final_den = den_tmp / g3;   // not needed */

    /* apply sign (-1)^(n+1) */
    seq_t sign = ((n + 1) & 1) ? (seq_t)(-1) : (seq_t)(1);
    return (sign == (seq_t)1) ? final_num : -final_num;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal d (d >= 1) such that d*(d+1)/2 > n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 <= n) ++d;

    idx_t offset = n - (d * (d - 1)) / 2;   /* position inside diagonal [0 .. d-1] */
    idx_t n_param = d - 1 - offset;         /* row index of T */
    idx_t k_param = offset + 1;             /* column index of T, k >= 1 */

    return T_numerator(n_param, k_param);
}
