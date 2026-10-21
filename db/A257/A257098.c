/*
 * A257098  From square root of the inverse of Riemann zeta function: form
 *          Dirichlet series Sum b(n)/n^x whose square is 1/zeta; sequence
 *          gives numerator of b(n).
 * https://oeis.org/A257098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, frac
 * Formula:   a(n) = numerator(b(n)) where b(n) is the coefficient in the
 *            Dirichlet series expansion ∑ b(n)/n^x = (ζ(x))^(–1/2). The
 *            function is multiplicative; for a prime power p^e the
 *            coefficient satisfies f(0) = 1, f(1) = –1/2, f(e) = –½ ·
 *            Σ_{i=1}^{e-1} f(i)·f(e–i) (e ≥ 2) Each f(e) is stored as a
 *            reduced...
 * Range:     i8 n <= 255, i16 n <= 8191, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* static cache for the numerator and denominator exponent tables  */
static seq_t  *num_arr  = NULL;      /* num[e]   */
static unsigned *den_arr = NULL;     /* den[e]   */
static size_t  max_precalc = 0;      /* number of entries currently allocated */

/* extend precalculation tables up to exponent `upto` (inclusive) */
static void extend_precalc(size_t upto)
{
    if (upto < max_precalc)
        return;

    size_t newSize = upto + 1;
    seq_t  *newNum  = (seq_t *)realloc(num_arr,  newSize * sizeof(seq_t));
    unsigned *newDen = (unsigned *)realloc(den_arr, newSize * sizeof(unsigned));

    /* compute missing entries from max_precalc to upto */
    for (size_t e = max_precalc; e <= upto; ++e)
    {
        if (e == 0)               /* f(0) = 1  */
        {
            newNum[e] = (seq_t)1;
            newDen[e] = 0;
        }
        else if (e == 1)          /* f(1) = –1/2 */
        {
            newNum[e] = (seq_t)-1;
            newDen[e] = 1;
        }
        else                      /* e ≥ 2  */
        {
            /* find maximum denominator exponent among the product terms */
            unsigned maxExp = 0;
            for (size_t i = 1; i < e; ++i)
            {
                unsigned exp_sum = newDen[i] + newDen[e - i];
                if (exp_sum > maxExp)
                    maxExp = exp_sum;
            }

            /* sum the terms with a common denominator 2^maxExp */
            seq_t sumNum = (seq_t)0;
            for (size_t i = 1; i < e; ++i)
            {
                seq_t termNum = newNum[i] * newNum[e - i];
                unsigned shift = maxExp - (newDen[i] + newDen[e - i]);
                termNum <<= shift;                 /* shift to common denominator */
                sumNum += termNum;
            }

            /* f(e) = –½ · (sumNum / 2^maxExp) = –sumNum / 2^(maxExp+1) */
            newNum[e] = -sumNum;
            newDen[e] = maxExp + 1;

            /* reduce the fraction by removing common factors of 2 */
            while ((newNum[e] % 2) == 0)
            {
                newNum[e] = newNum[e] / 2;
                newDen[e]--;
            }
        }
    }

    num_arr   = newNum;
    den_arr   = newDen;
    max_precalc = newSize;
}

/* sequence function:  a(n) */
seq_t a(idx_t n)
{
    /* factorise n – factor_u64 writes distinct primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   num_factors = 0;

    factor_u64((uint64_t)n, primes, exps, &num_factors);

    /* find the maximum exponent in the factorisation */
    uint64_t max_exp = 0;
    for (size_t i = 0; i < num_factors; ++i)
        if (exps[i] > max_exp)
            max_exp = exps[i];

    /* ensure tables contain entries for all these exponents */
    extend_precalc(max_exp);

    /* product of the numerators */
    seq_t product = (seq_t)1;
    for (size_t i = 0; i < num_factors; ++i)
        product *= num_arr[exps[i]];   /* num_arr[0] is 1, so n=1 gives 1 */

    return product;
}
