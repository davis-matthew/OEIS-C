/*
 * A341431  a(n) is the numerator of the asymptotic density of numbers
 *          divisible by their last digit in base n.
 * https://oeis.org/A341431
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, frac, easy
 * Formula:   a(n) = numerator of the lowest–term fraction * (1/n) *
 *            Sum_{k=1}^{n-1} gcd(k,n)/k, * i.e. a(n) = p / gcd(p,n) where p/q
 *            = Sum_{k=1}^{n-1} gcd(k,n)/k * and p,q are coprime. * * The
 *            implementation keeps the fraction p/q reduced at each step. *
 *            After the summation the factor 1/n is applied by dividi...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 26, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* undefined for n < 2 in OEIS context */
    if (n < 2)
        return 0;

    /* p/q = Sum gcd(k,n)/k  with p,q coprime */
    seq_t num = 0;   /* p */
    seq_t den = 1;   /* q */

    const seq_t n_se = (seq_t)n;

    for (idx_t k = 1; k < n; ++k)
    {
        /* term = gcd(k,n) / k  →  1/(k/g)   because gcd(k,n) | k   */
        seq_t g = gcd((seq_t)k, n_se);
        seq_t term_den = (seq_t)k / g;   /* numerator is 1 */

        if (num == 0)
        {
            num = 1;
            den = term_den;
            continue;
        }

        /* Add num/den + 1/term_den  =>  (num*term_den/g + den/g) / (den*term_den/g) */
        seq_t g2 = gcd(den, term_den);
        seq_t mul1 = term_den / g2;      /* term_den/g2   */
        seq_t mul2 = den / g2;           /* den/g2        */

        seq_t new_num = num * mul1 + mul2;
        seq_t new_den = den * mul1;

        seq_t g3 = gcd(new_num, new_den);
        num = new_num / g3;
        den = new_den / g3;
    }

    /* divide by n:   num/(den*n)  →  (num/g) / (den*(n/g))  where g=gcd(num,n)   */
    seq_t g_final = gcd(num, n_se);
    num /= g_final;

    /* num is now the desired a(n) */
    return num;
}
