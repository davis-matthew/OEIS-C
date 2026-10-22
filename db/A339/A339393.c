/*
 * A339393  Denominators of the probability that when a stick is broken up at
 *          n-1 points independently and uniformly chosen at random along its
 *          length there exist 3 of the n pieces that can form a triangle.
 * https://oeis.org/A339393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(1 - Product_{k=2..n} k/(Fib(k+2)-1))
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 12, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* Undefined below offset – return safe value */
        return (seq_t)1;
    if (n == 1)         /* a(1) = 1 by definition */
        return (seq_t)1;

    /* Running product P = N / D, starting with 1/1 */
    seq_t N = 1;
    seq_t D = 1;

    /* Fibonacci state:  F1 = 1, F2 = 1 */
    seq_t f_prev = 1, f_curr = 1;

    /* Loop over i = 3 .. n+2.
       i corresponds to Fibonacci index F_i.
       When i >= 4, we have k = i-2 >= 2 and need
       factor = k / (F_i - 1). */
    for (idx_t i = 3; i <= n + 2; ++i) {
        /* Compute next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;                /* f_curr now holds F_i */

        if (i >= 4) {                    /* k = i-2 >= 2 */
            idx_t k = i - 2;
            seq_t factor_num = (seq_t)k;          /* numerator: k */
            seq_t factor_den = f_curr - 1;        /* denominator: F_i - 1 */

            /* Reduce the factor fraction itself */
            seq_t g = gcd(factor_num, factor_den);
            if (g != 1) {
                factor_num /= g;
                factor_den /= g;
            }

            /* Reduce factor numerator with current denominator D */
            g = gcd(factor_num, D);
            if (g != 1) {
                factor_num /= g;
                D /= g;
            }

            /* Reduce factor denominator with current numerator N */
            g = gcd(N, factor_den);
            if (g != 1) {
                N /= g;
                factor_den /= g;
            }

            /* Multiply the simplified pieces into the product */
            N *= factor_num;
            D *= factor_den;
        }
    }

    /* Now compute 1 - P = (D - N) / D and return its denominator */
    seq_t diff = D - N;                    /* D >= N for all n >= 1 */
    seq_t g = gcd(diff, D);
    return D / g;
}
