/*
 * A284637  Discriminants of polynomials having Fibonacci numbers (A000045)
 *          for coefficients, P_n(x) = Sum_{k=1..n} F(k)*x^(2n-1-k) +
 *          Sum_{k=1..(n-1)} (-1)^k*F(n-k)*x^(n-k-1); a(1) = 1.
 * https://oeis.org/A284637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n-1) * n^(2n-4) * F_n^(2n-4) for n>=2, a(1) = 1 where
 *            F_n is the nth Fibonacci number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/big_pow.h"

/* Compute a(n) according to the closed form above. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t fib_n = fib(n);          /* Fibonacci F_n   */
    idx_t exp   = 2 * n - 4;       /* exponent for n and F_n (>=0 for n>=2) */

    seq_t pow5   = big_pow((seq_t)5,   (seq_t)(n - 1));
    seq_t pown   = big_pow((seq_t)n,   (seq_t)exp);
    seq_t powfib = big_pow(fib_n,       (seq_t)exp);

    return pow5 * pown * powfib;
}
