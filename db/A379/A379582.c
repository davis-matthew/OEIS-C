/*
 * A379582  Denominators of the partial alternating sums of the reciprocals of
 *          the powerfree part function (A055231).
 * https://oeis.org/A379582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of Sum_{k=1..n} (-1)^{k+1}/A055231(k), where
 *            A055231(k) = ∏{prime p|k : exponent of p in k is exactly 1} p.
 *            The function computes the partial alternating sum of reciprocals
 *            of the powerfree part function, reduces it to lowest terms, and
 *            returns its denominator
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 29, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   build_spf, gcd, powerfree_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"
#include "../utils/powerfree_u.h"

static idx_t *spf = NULL;           /* smallest prime factor table */
static idx_t  spf_limit = 0;        /* current size of spf table */

seq_t a(idx_t n)
{
    /* Ensure SPF table up to n */
    if (spf == NULL || spf_limit < n)
    {
        if (spf != NULL) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    seq_t num = 0;    /* absolute value of numerator */
    seq_t den = 1;    /* denominator (always positive) */
    int   sign = 1;   /* sign of the current value: 1 or -1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t f = powerfree_u(k, spf);          /* A055231(k) */
        int   term_sign = (k & 1) ? 1 : -1;      /* (-1)^{k+1} */

        /* lcm of current denominator and f */
        seq_t g = gcd(den, f);                  /* gcd(den, f) */
        seq_t mul_den = f / g;                   /* l/den = f/g */
        seq_t mul_f   = den / g;                 /* l/f   = den/g */

        seq_t A = num * mul_den;                /* |num| * (l/den) */
        seq_t B = mul_f;                          /* 1 * (l/f)   */

        /* Add signed fractions: (sign*num)/(den) + (term_sign*1)/(f) */
        seq_t new_num;
        int   new_sign;
        if (sign == term_sign)                  /* same sign */
        {
            new_sign = sign;
            new_num  = A + B;
        }
        else                                    /* different signs */
        {
            if (A >= B)
            {
                new_sign = sign;
                new_num  = A - B;
            }
            else
            {
                new_sign = term_sign;
                new_num  = B - A;
            }
        }

        /* Reduce the resulting fraction */
        if (new_num == 0)                       /* sum is zero */
        {
            num = 0;
            den = 1;
            sign = 1;
        }
        else
        {
            seq_t new_den = den / g * f;         /* lcm(den, f) */
            seq_t h = gcd(new_num, new_den);
            num  = new_num / h;
            den  = new_den / h;
            sign = new_sign;
        }
    }

    /* The denominator is always positive */
    return den;
}
