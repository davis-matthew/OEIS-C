/*
 * A259218  Number of (n+1) X (4+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 8 + 11 * F(n+3) (F is the Fibonacci sequence)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* For the OEIS offset a(1) = 45, so n ≥ 1 */
    if (n == 0) return 0;            /* undefined for the sequence, but safe default */

    /* 2^(n+1) */
    seq_t pow_term = pow2(n + 1);

    /* Fibonacci term F(n+3) */
    seq_t fib_term = fib(n + 3);

    /* 11 * F(n+3) */
    seq_t fib_mult = fib_term * (seq_t)11;

    /* Final formula */
    return pow_term + (seq_t)8 + fib_mult;
}
