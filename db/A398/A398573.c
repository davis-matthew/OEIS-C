/*
 * A398573  Numerators of the coefficients in the expansion of (1 - x -
 *          x^2)^(-3/2).
 * https://oeis.org/A398573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the nth coefficient in the expansion * of (1
 *            - x - x^2)^(-3/2). The coefficient satisfies * c[0] = 1, * c[1]
 *            = 3/2, * c[n] = (2n+1)/(2n) * c[n-1] + (n+1)/n * c[n-2] for n ≥
 *            2. * a(n) is the numerator of c[n] when written in lowest terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic integer utilities used by the recurrence */
#include <stdint.h>
#include <stdbool.h>

/* A small helper to compute gcd for a __int128 value.
 * The algorithm works for both 64‑bit and 128‑bit seq_t values
 * because all intermediate products are held in unsigned __int128. */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main function computing a(n) ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Numerator / denominator of the previous two coefficients. */
    seq_t num0 = (seq_t)1;   /* c0 = 1/1   */
    seq_t den0 = (seq_t)1;

    seq_t num1 = (seq_t)3;   /* c1 = 3/2   */
    seq_t den1 = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = c_{i-1} * (2i+1)/(2i)  */
        unsigned __int128 t1_num = (unsigned __int128)num1 * (unsigned __int128)(2 * i + 1);
        unsigned __int128 t1_den = (unsigned __int128)den1 * (unsigned __int128)(2 * i);

        /* term2 = c_{i-2} * (i+1)/i      */
        unsigned __int128 t2_num = (unsigned __int128)num0 * (unsigned __int128)(i + 1);
        unsigned __int128 t2_den = (unsigned __int128)den0 * (unsigned __int128)i;

        /* sum = t1_num/t1_den + t2_num/t2_den */
        unsigned __int128 new_num = t1_num * t2_den + t2_num * t1_den;
        unsigned __int128 new_den = t1_den * t2_den;

        /* Reduce the fraction to lowest terms. */
        unsigned __int128 g = gcd128(new_num, new_den);
        new_num /= g;
        new_den /= g;

        /* Cast back to seq_t (safe because the reduced values fit in seq_t). */
        num0 = num1;
        den0 = den1;
        num1 = (seq_t)new_num;
        den1 = (seq_t)new_den;
    }

    /* The numerator of the nth coefficient is the desired sequence value. */
    return num1;
}
