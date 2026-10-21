/*
 * A279334  Numerators of coefficients in expansion of exp(x)/(1 - x - x^2).
 * https://oeis.org/A279334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the coefficient of x^n in the expansion of
 *            exp(x)/(1 - x - x^2). The coefficient is c_n = Σ_{m=0}^{n}
 *            F_{n-m+1} / m! (F_k are Fibonacci numbers with F_1=1, F_2=1).
 *            a(n) is obtained by reducing the fraction c_n to lowest terms
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Fibonacci numbers up to F_{n+1} */
    seq_t *fib = (seq_t *)malloc((n + 2) * sizeof(seq_t));
    if (!fib) return 0;            /* allocation failed */
    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* sum S = Σ_{m=0}^{n} F_{n-m+1} * (n! / m!) */
    seq_t sum  = 0;
    seq_t mult = fact;                /* n! / 0! */
    for (idx_t m = 0; m <= n; ++m) {
        seq_t F = fib[n - m + 1];
        sum += F * mult;
        if (m < n)                 /* update mult to n!/(m+1)! */
            mult /= (seq_t)(m + 1);
    }

    /* reduce the fraction sum/n! to lowest terms */
    seq_t g = gcd(sum, fact);
    seq_t numerator = sum / g;

    free(fib);
    return numerator;
}
