/*
 * A250743  Numerator of the harmonic mean of the first n positive Fibonacci
 *          numbers.
 * https://oeis.org/A250743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the harmonic mean of the first n positive *
 *            Fibonacci numbers. * The harmonic mean H of 1/F_1,…,1/F_n is * n
 *            * ----------------- * Σ_{i=1..n} 1/F_i * Let Σ 1/F_i = A/B in
 *            lowest terms. Then * H = n * B / A * Since A and B are coprime,
 *            the reduced fraction is obtained by * divi...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /*  Reduce the sum Σ 1/F_i to a fraction A/B. */
    seq_t A = 0;
    seq_t B = 1;

    /* Fibonacci generation: a = F_{i-1}, b = F_i   (F_0 = 0, F_1 = 1) */
    seq_t fib_prev = 0;
    seq_t fib_cur  = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t Fi = fib_cur;          /* F_i */

        /* New numerator and denominator before reduction. */
        seq_t newA = A * Fi + B;
        seq_t newB = B * Fi;

        /* Reduce by gcd to keep numbers small. */
        seq_t g = gcd(newA, newB);
        A = newA / g;
        B = newB / g;

        /* Advance Fibonacci */
        seq_t next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = next;
    }

    /* Compute g = gcd(n, A) */
    seq_t sn   = (seq_t)n;           /* cast index to seq_t */
    seq_t g    = gcd(sn, A);
    seq_t sn_div_g = sn / g;

    /* Numerator of the reduced harmonic mean. */
    seq_t ans = B * sn_div_g;
    return ans;
}
