/*
 * A250744  Denominator of the harmonic mean of the first n positive Fibonacci
 *          numbers.
 * https://oeis.org/A250744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A250744
 *  Formula: a(n) = denominator of the harmonic mean of the first n Fibonacci numbers,
 *  i.e. a(n) = denominator of n / (1/F1 + 1/F2 + … + 1/Fn).
 *  The function returns the value for n ≥ 1; for n = 0 it returns 0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1 – no defined value for n = 0 */

    /* p/q will hold the sum of the reciprocals of the first i Fibonacci numbers */
    seq_t p = 0;
    seq_t q = 1;

    /* Fibonacci numbers: f_prev = F(i-1), f_curr = F(i) */
    seq_t f_prev = 1;
    seq_t f_curr = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t fib;
        if (i <= 2) {                   /* F1 = 1, F2 = 1 */
            fib = 1;
        } else {
            fib = f_prev + f_curr;      /* F(i) = F(i-1) + F(i-2) */
            f_prev = f_curr;
            f_curr = fib;
        }

        /* Add 1/fib to the current sum: p/q + 1/fib = (p*fib + q)/(q*fib) */
        seq_t new_p = p * fib + q;
        seq_t new_q = q * fib;

        /* Reduce the fraction to keep numbers small */
        seq_t g = gcd(new_p, new_q);
        p = new_p / g;
        q = new_q / g;
    }

    /* Harmonic mean H = n / (p/q) = (n * q) / p.  Reduce the fraction. */
    seq_t sqn = (seq_t)n;
    seq_t g2 = gcd(p, sqn * q);
    return p / g2;                       /* denominator after reduction */
}
