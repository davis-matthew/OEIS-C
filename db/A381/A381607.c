/*
 * A381607  For any nonnegative integer n with ternary expansion Sum_{k >= 0}
 *          t_k * 3^k, a(n) = Sum_{k >= 0} t_k * A000045(2*k + 2).
 * https://oeis.org/A381607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k>=0} t_k * A000045(2*k + 2) where t_k are the
 *            base-3 digits of n. This implements the definition of OEIS
 *            A381607
 * Range:     u8 n <= 393, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* precompute Fibonacci numbers up to at least F(82) */
    static seq_t fib[128];
    static int fib_ready = 0;

    if (!fib_ready) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        for (int i = 2; i < 128; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        fib_ready = 1;
    }

    seq_t sum = (seq_t)0;
    idx_t tmp = n;
    int k = 0;

    while (tmp) {
        unsigned t = (unsigned)(tmp % 3);
        if (t) {
            seq_t coeff = fib[2*k + 2];      /* F(2k+2) */
            sum += coeff * (seq_t)t;
        }
        tmp /= 3;
        ++k;
    }

    return sum;
}
