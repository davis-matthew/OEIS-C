/*
 * A343539  a(n) = (2*n+1)*Lucas(2*n+1).
 * https://oeis.org/A343539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*Lucas(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling method for Fibonacci numbers
   Computes F_n and F_{n+1} recursively */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);           /* a = F_k,   b = F_{k+1},  k = n/2 */

    /* F_{2k}   = F_k * (2*F_{k+1} - F_k) */
    seq_t two_b_minus_a = (b << 1) - a;
    seq_t c = a * two_b_minus_a;

    /* F_{2k+1} = F_k^2 + F_{k+1}^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci pair for index 2*n */
    idx_t m = 2 * n;
    seq_t f_even, f_even1;                /* f_even  = F_{2n},  f_even1 = F_{2n+1} */
    fib_pair(m, &f_even, &f_even1);

    /* Lucas number L_{2n+1} = 2*F_{2n} + F_{2n+1} */
    seq_t luc = f_even * 2 + f_even1;

    /* a(n) = (2*n+1) * L_{2n+1} */
    seq_t result = (seq_t)(2 * n + 1) * luc;
    return result;
}
